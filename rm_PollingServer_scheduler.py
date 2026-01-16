# rm_PollingServer_scheduler.py
# ------------------------------------------------------------
# RM + Polling Server (PS) "puro" (Buttazzo-style):
# - Il PS "polla" solo agli istanti k*Ts.
# - Se al polling instant NON c'e' niente da servire -> SALTA IL TURNO:
#   budget perso per tutto il periodo (ps_budget=0 fino al prossimo k*Ts).
# - Se arrivano aperiodici dopo il polling instant, aspettano il prossimo periodo.
#
# Plot:
# - Ogni task ha una "retta" (baseline) orizzontale.
# - Se il task gira, le barre sono disegnate SOPRA la retta.
# - Release: linea verticale nera sulla corsia del task.
# - Fine job periodici: X nera sopra la barra.
# - Arrivi aperiodici: testo "Ta=..  C=.." sulla retta del PS (senza marker).
# ------------------------------------------------------------

import matplotlib.pyplot as plt
from math import gcd
from functools import reduce
from collections import deque

# ============================================================
# CONFIGURAZIONE TASK SET (C e T in ms; step di simulazione = 1 ms)
# ============================================================

tasksB1 = [
    ("Batteria", 1, 200),
    ("Temperatura", 1, 200),
    ("Motore", 5, 20),
    ("Comm", 12, 50),
]

tasksB2 = [
    ("Controller", 2, 20),
    ("Gyro", 4, 40),
    ("Sonar", 25, 100),
    ("Comm", 12, 50),
]

# Scegli task set
tasks = tasksB1  # <-- CAMBIA QUI

# Polling Server (nome, Cs, Ts) in ms
PS = ("PS", 3, 20)  # <-- CAMBIA QUI: budget Cs ogni Ts

# Aperiodici: (nome, Ta, C) in ms
aperiodic_jobs = [
    ("A1", 7, 2),
    ("A2", 23, 4),
    ("A3", 41, 1),
]  # <-- CAMBIA QUI


# ============================================================
# Utility
# ============================================================

def lcm(a, b):
    return a * b // gcd(a, b)

def hyperperiod(periods):
    return reduce(lcm, periods)

def rm_utilization(periodic_tasks, ps=None):
    U = sum(C / T for _, C, T in periodic_tasks)
    if ps is not None:
        _, Cs, Ts = ps
        U += Cs / Ts
    return U

def liu_layland_bound(n):
    return n * (2 ** (1 / n) - 1)

# -------------------------
# Dimensionamento Polling Server: Us <= Us_max (slide con P = Π(Ui+1))
# -------------------------
ps_name, Cs, Ts = PS

if Cs <= 0 or Ts <= 0:
    raise ValueError("PS: Cs e Ts devono essere > 0")
if Cs > Ts:
    raise ValueError("PS: deve valere Cs <= Ts (altrimenti Us>1)")

Ui = [C / T for _, C, T in tasks]  # SOLO periodici (escludi PS)
P = 1.0
for u in Ui:
    P *= (u + 1.0)

Us = Cs / Ts
Us_max = (2.0 - P) / P   # = 2/P - 1

print("\n--- Dimensionamento Polling Server (bound con P) ---")
print("Ui (periodici):", [round(u, 6) for u in Ui])
print(f"P = Π(Ui+1) = {P:.6f}")
print(f"Us = Cs/Ts = {Us:.6f} ({Us*100:.2f}%)")
print(f"Us_max     = {Us_max:.6f} ({Us_max*100:.2f}%)")

if Us_max < 0:
    print("✘ ATTENZIONE: Us_max < 0 -> già i periodici sono troppo 'pesanti' per lasciare banda al server con questo bound.")
elif Us <= Us_max:
    print("✔ OK: il tuo Polling Server soddisfa Us <= Us_max (condizione sufficiente).")
else:
    print("✘ NO: il tuo Polling Server NON soddisfa Us <= Us_max (nessuna garanzia con questo bound).")


# ============================================================
# Preparazione simulazione
# ============================================================

ps_name, ps_C, ps_T = PS

# Periodi per iperperiodo (includo PS)
H = hyperperiod([T for _, _, T in tasks] + [ps_T])

# Test sufficiente (Liu & Layland) includendo PS come task periodico aggiuntivo
U = rm_utilization(tasks, PS)
n = len(tasks) + 1
U_ll = liu_layland_bound(n)

print(f"Iperperiodo H = {H} ms")
print(f"Utilizzazione totale (incl. PS) U = {U:.3f} ({U*100:.1f}%)")
print(f"Limite teorico RM (n={n}) U_ll = {U_ll:.3f} ({U_ll*100:.1f}%)")
print("✔ Test sufficiente: schedulabile" if U <= U_ll else "✘ Test sufficiente NON garantisce schedulabilità")

# ============================================================
# Simulazione RM + Polling Server "puro" (step = 1 ms)
# ============================================================

time = 0
jobs = []                # job periodici attivi
schedule = []            # (t, task_name) per ogni ms
finish_marks = []        # (finish_time, task_name) per periodici
job_id_counter = 0

# release_times per plot
release_times = {name: [] for name, _, _ in tasks}
release_times[ps_name] = []

# Coda aperiodici
aper_q = deque()
aper_list = sorted(aperiodic_jobs, key=lambda x: x[1])
aper_idx = 0

# Stato PS
ps_budget = 0  # budget disponibile nel periodo corrente (0 => PS non runnable)

# (Opzionale) deadline miss check per periodici: D=T
deadline_misses = []  # (task_name, release, deadline, finish) se vuoi usarlo

while time < H:
    # --------------------------------------------------------
    # 0) (facoltativo) deadline miss check "per job" dei periodici
    #    Qui non tracciamo finish per ogni release separata,
    #    quindi lasciamo disattivato per semplicità.
    # --------------------------------------------------------

    # --------------------------------------------------------
    # 1) Arrivi aperiodici
    #    NOTA: l'ordine qui è: ARRIVI -> poi POLLING instant.
    #    Quindi un aperiodico con Ta == k*Ts è visibile al polling a Ta.
    #    Se vuoi l'opposto, sposta questo blocco dopo il polling.
    # --------------------------------------------------------
    while aper_idx < len(aper_list) and aper_list[aper_idx][1] == time:
        aname, at, aC = aper_list[aper_idx]
        aper_q.append({"name": aname, "remaining": aC, "arrival": at})
        aper_idx += 1

    # --------------------------------------------------------
    # 2) Release periodici normali
    # --------------------------------------------------------
    for name, C, T in tasks:
        if time % T == 0:
            release_times[name].append(time)
            jobs.append({
                "id": job_id_counter,
                "name": name,
                "remaining": C,
                "period": T,
                "release": time,
            })
            job_id_counter += 1

    # --------------------------------------------------------
    # 3) Polling instant del PS (release logico)
    #    PS "puro": se non c'e' niente ORA, il budget è perso fino al prossimo periodo.
    # --------------------------------------------------------
    if time % ps_T == 0:
        release_times[ps_name].append(time)
        ps_budget = ps_C if len(aper_q) > 0 else 0

    # --------------------------------------------------------
    # 4) Selezione RM con PS
    #    - I periodici normali competono per RM (T minimo).
    #    - PS compete con periodo Ts, ma e' runnable SOLO se ps_budget>0 e aper_q non vuota.
    #    - Tie-break a parità periodo: il task periodico "normale" vince (PS sotto).
    # --------------------------------------------------------
    ready_periodic = [j for j in jobs if j["remaining"] > 0]
    periodic_choice = min(ready_periodic, key=lambda j: j["period"]) if ready_periodic else None

    ps_runnable = (ps_budget > 0 and len(aper_q) > 0)

    run_ps = False
    if ps_runnable:
        if periodic_choice is None:
            run_ps = True
        else:
            if ps_T < periodic_choice["period"]:
                run_ps = True
            elif ps_T == periodic_choice["period"]:
                run_ps = False  # tie-break: periodico normale prima del PS
            else:
                run_ps = False

    if run_ps:
        # 1 ms di PS: consumo budget e 1 ms dell'aperiodico in testa
        ps_budget -= 1
        aper_q[0]["remaining"] -= 1
        schedule.append((time, ps_name))

        if aper_q[0]["remaining"] == 0:
            aper_q.popleft()

    elif periodic_choice is not None:
        periodic_choice["remaining"] -= 1
        schedule.append((time, periodic_choice["name"]))

        if periodic_choice["remaining"] == 0:
            finish_marks.append((time + 1, periodic_choice["name"]))

    else:
        schedule.append((time, "IDLE"))

    # Rimuovi periodici completati
    jobs = [j for j in jobs if j["remaining"] > 0]

    time += 1


# ============================================================
# PLOT (rette per task, barre sopra la retta, Ta/C su PS)
# ============================================================

fig, ax = plt.subplots(figsize=(16, 5))

# Periodi per ordine RM (PS incluso)
period_of = {name: T for (name, _, T) in tasks}
period_of[ps_name] = ps_T

# Task presenti nello schedule (escludo IDLE)
present = sorted({task for _, task in schedule if task != "IDLE"})

known = [t for t in present if t in period_of]
unknown = [t for t in present if t not in period_of]

# RM order: T crescente (priorità alta), tie-break: PS sotto a parità
known_sorted = sorted(known, key=lambda n: (period_of[n], 1 if n == ps_name else 0))
task_names = known_sorted + unknown

# Layout
lane = 1.25        # distanza tra rette
bar_h = 0.60       # altezza barre
bar_offset = 0  # barre sopra la retta
N = len(task_names)

# y della retta: priorità alta in alto
y_line = {name: (N - 1 - i) * lane for i, name in enumerate(task_names)}

# colori
colors = plt.cm.tab10(range(N))
color_map = {name: colors[i] for i, name in enumerate(task_names)}

# 1) rette (baseline)
for name in task_names:
    y = y_line[name]
    ax.hlines(y, 0, H, colors='0.65', linewidth=1.0)

# 2) barre di esecuzione (sopra baseline)
for t, task in schedule:
    if task == "IDLE":
        continue
    if task not in y_line:
        continue
    y = y_line[task]
    ax.broken_barh([(t, 1)], (y + bar_offset, bar_h),
                   facecolors=color_map[task], edgecolors='none')

# 3) release bars: linee verticali nere per ogni release
for name in task_names:
    xs = release_times.get(name, [])
    if not xs:
        continue
    y = y_line[name]
    ax.vlines(xs, y - 0.05, y + bar_offset + bar_h + 0.20, colors='black', linewidth=1.6)

# 4) finish marks (solo periodici normali)
for ft, name in finish_marks:
    if name not in y_line:
        continue
    y = y_line[name]
    ax.plot(ft, y + bar_offset + bar_h + 0.10,
            marker='x', color='black', markersize=6, linestyle='None', zorder=20)

# 5) annotazioni arrivi aperiodici su PS: "Ta=.. C=.."
if ps_name in y_line:
    yps = y_line[ps_name]
    dy_list = [0.32, -0.34, 0.48, -0.52]  # alterno su/giu per evitare sovrapposizioni
    for k, (_aname, Ta, aC) in enumerate(aperiodic_jobs, start=1):
        dy = dy_list[(k - 1) % len(dy_list)]
        ax.text(Ta, yps + dy,
                f"Ta={Ta}  C={aC}",
                ha='center', va='center', fontsize=8,
                bbox=dict(boxstyle="round,pad=0.15", fc="white", ec="black", lw=0.6),
                zorder=30)

# Asse Y: etichette sulle rette
ax.set_yticks([y_line[n] for n in task_names])
ax.set_yticklabels(task_names)

# Asse X e titolo
ax.set_xlim(0, H)
ax.set_xlabel("Tempo [ms]")
ax.set_title("RM + Polling Server (puro): rette per task | barre sopra la retta | release(|) | fine(X) | aperiodici: Ta,C su PS")

# Ticks X leggibili
if H <= 200:
    tick_step = 10
elif H <= 1000:
    tick_step = 50
elif H <= 5000:
    tick_step = 250
else:
    tick_step = 500

ax.set_xticks(range(0, H + 1, tick_step))
ax.set_xticks(range(0, H + 1, 1), minor=True)
ax.tick_params(axis='x', which='minor', bottom=False, labelbottom=False)

# Griglie
ax.grid(True, which='minor', axis='x', linestyle='-', linewidth=0.12, color='0.88')
ax.grid(True, which='major', axis='x', linestyle='-', linewidth=0.6, color='0.75')
ax.grid(True, which='major', axis='y', linestyle='--', linewidth=0.4, color='0.80')

plt.tight_layout()
plt.show()
