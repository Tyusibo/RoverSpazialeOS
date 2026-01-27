# rm_PollingServer_scheduler.py
# ------------------------------------------------------------
# RM + Polling Server (PS) "puro" (Buttazzo-style) - VERSIONE JOB-BASED
#
# Questa versione riflette la tua implementazione CMSIS-RTOS2:
# - Il thread PS si "attiva" SOLO al rilascio periodico (k*Ts).
# - All'attivazione, se non ci sono richieste pendenti, non fa lavoro e si riaddormenta.
# - Se ci sono richieste, le serve consumando AL PIÙ Cs ms (budget) e poi termina il job.
# - Se la coda si svuota prima, termina subito (non resta runnable).
# - Se arrivano nuove richieste dopo che il PS ha terminato, aspettano il prossimo rilascio.
#
# Callback aperiodiche dipendenti dallo start dei task periodici:
# - Quando un job periodico di lettura inizia a eseguire (primo ms), schedula callback aperiodiche.
# - Controller: +2ms
# - Gyro: +4ms
# - Sonar: +[2..25]ms oppure triplo Left/Front/Right con delay diversi (parametro).
#
# Plot:
# - Baseline per task (rette).
# - Barre che partono dalla retta.
# - Release: linea verticale nera.
# - Fine job periodici: end-cap (ultimo rettangolino) più evidente (alto e/o colore).
# - Callback: segmento start→Ta disegnato nella corsia del task periodico sorgente.
# - PS: ms colorati in base alla callback servita (sorgente distinta, incl. sonar triplo).
# - Legenda in basso a destra.
# ------------------------------------------------------------

import matplotlib.pyplot as plt
from math import gcd
from functools import reduce
from collections import deque
import random
from matplotlib.lines import Line2D
from matplotlib.patches import Patch

# ============================================================
# CONFIGURAZIONE (Board2)
# ============================================================

tasksB2 = [
    ("Controller", 1, 20),
    ("Gyro",       1, 40),
    ("Sonar",      2, 100),
    ("Comm",      12, 50),
]
tasks = tasksB2

# Polling Server (nome, Cs, Ts) in ms
PS = ("PS", 3, 10)

# -------------------------
# Dinamica Sonar: 1 callback oppure 3 callback (Left/Front/Right)
# -------------------------
SONAR_TRIPLE_CALLBACKS = True   # False => una sola callback "Sonar"

# Callback singole (delay_min, delay_max, C_callback)
SENSOR_CALLBACKS_SINGLE = {
    "Controller": (2, 2, 1),
    "Gyro":       (4, 4, 1),
    "Sonar":      (2, 25, 1),
}

# Sonar triplo (delay diversi per generare arrivi distinti)
SONAR_CALLBACKS_TRIPLE = {
    "SonarLeft":  (2, 10, 1),
    "SonarFront": (4, 18, 1),
    "SonarRight": (6, 25, 1),
}

# Delay worst-case (max) oppure random nel range (ripetibile con seed)
USE_WORST_CASE_DELAY = True
RANDOM_SEED = 1

# Tie-break Ts == Ttask:
# True  => periodico normale vince su PS (PS sotto)
# False => PS vince a parità
PERIODIC_WINS_TIE = True

# -------------------------
# Fine job periodici: end-cap più visibile
# -------------------------
FINISH_CAP_MODE = "both"         # "tall" | "color" | "both"
FINISH_CAP_COLOR = "black"
FINISH_CAP_HEIGHT = 0.95         # altezza cap (<= lane)
# ============================================================


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

def ps_bound_P(tasks_periodic, PS_tuple):
    _, Cs, Ts = PS_tuple
    Ui = [C / T for _, C, T in tasks_periodic]
    P = 1.0
    for u in Ui:
        P *= (u + 1.0)
    Us = Cs / Ts
    Us_max = (2.0 - P) / P
    return Ui, P, Us, Us_max


# ============================================================
# Costruisci la lista "sorgenti callback" (per colori/legenda)
# ============================================================

def callback_sources():
    src = ["Controller", "Gyro"]
    if SONAR_TRIPLE_CALLBACKS:
        src += ["SonarLeft", "SonarFront", "SonarRight"]
    else:
        src += ["Sonar"]
    return src

CALLBACK_SOURCES = callback_sources()


# ============================================================
# Check dimensionamento server (bound con P) + LL (extra)
# ============================================================

ps_name, Cs, Ts = PS

if Cs <= 0 or Ts <= 0:
    raise ValueError("PS: Cs e Ts devono essere > 0")
if Cs > Ts:
    raise ValueError("PS: deve valere Cs <= Ts (altrimenti Us>1)")

Ui, P, Us, Us_max = ps_bound_P(tasks, PS)

print("\n--- Dimensionamento Polling Server (bound con P) ---")
print("Ui (periodici):", [round(u, 6) for u in Ui])
print(f"P = Π(Ui+1) = {P:.6f}")
print(f"Us = Cs/Ts = {Us:.6f} ({Us*100:.2f}%)")
print(f"Us_max     = {Us_max:.6f} ({Us_max*100:.2f}%)")
if Us_max < 0:
    print("✘ ATTENZIONE: Us_max < 0 -> bound non lascia spazio al server.")
elif Us <= Us_max:
    print("✔ OK: il tuo Polling Server soddisfa Us <= Us_max (condizione sufficiente).")
else:
    print("✘ NO: il tuo Polling Server NON soddisfa Us <= Us_max (nessuna garanzia con questo bound).")

H = hyperperiod([T for _, _, T in tasks] + [Ts])

U_tot = rm_utilization(tasks, PS)
n_ll = len(tasks) + 1
U_ll = liu_layland_bound(n_ll)

print(f"\nIperperiodo H = {H} ms")
print(f"Utilizzazione totale (incl. PS) U = {U_tot:.3f} ({U_tot*100:.1f}%)")
print(f"Limite teorico RM (n={n_ll}) U_ll = {U_ll:.3f} ({U_ll*100:.1f}%)")
print("✔ LL sufficiente: schedulabile" if U_tot <= U_ll else "✘ LL non garantisce schedulabilità")


# ============================================================
# Simulazione RM + Polling Server puro (JOB-BASED) + callback dipendenti
# ============================================================

if not USE_WORST_CASE_DELAY:
    random.seed(RANDOM_SEED)

time = 0
jobs = []             # job periodici attivi: {"id","name","remaining","period","release"}
schedule = []         # (t, task_name) ogni 1ms
job_id_counter = 0

release_times = {name: [] for name, _, _ in tasks}
release_times[ps_name] = []

# coda aperiodici (serviti dal PS)
aper_q = deque()      # {"id","src","remaining","arrival","C"}

# eventi futuri callback: (Ta, cb_id, src, C)
future_events = []

# per plot: link start→Ta (nella corsia del task periodico sorgente)
cb_links = []         # (src_callback, task_periodico_sorgente, t_start, Ta, C, cb_id)

# per PS colorato: t -> src_callback
ps_exec_src = {}

# fine job periodici: end-cap all'ultimo ms
finish_caps = []      # (t_last_exec, task_name)

# PS job-based (None o dict con remaining=Cs)
ps_job = None

# tracking start job periodici per generare callback una sola volta per job
started_job_ids = set()

cb_id_counter = 0

def choose_delay(dmin, dmax):
    return dmax if USE_WORST_CASE_DELAY else random.randint(dmin, dmax)

def enqueue_future_event(Ta: int, src_cb: str, Ccb: int, task_src: str, t_start: int):
    """Registra evento futuro + link per plot."""
    global cb_id_counter
    if Ta > H:
        return
    cb_id = cb_id_counter
    cb_id_counter += 1
    future_events.append((Ta, cb_id, src_cb, Ccb))
    cb_links.append((src_cb, task_src, t_start, Ta, Ccb, cb_id))

def schedule_callbacks_from_task(task_name: str, start_time: int):
    """Genera callback quando il task periodico inizia a eseguire."""
    if task_name == "Controller":
        dmin, dmax, Ccb = SENSOR_CALLBACKS_SINGLE["Controller"]
        Ta = start_time + choose_delay(dmin, dmax)
        enqueue_future_event(Ta, "Controller", Ccb, "Controller", start_time)

    elif task_name == "Gyro":
        dmin, dmax, Ccb = SENSOR_CALLBACKS_SINGLE["Gyro"]
        Ta = start_time + choose_delay(dmin, dmax)
        enqueue_future_event(Ta, "Gyro", Ccb, "Gyro", start_time)

    elif task_name == "Sonar":
        if SONAR_TRIPLE_CALLBACKS:
            for cb_name, (dmin, dmax, Ccb) in SONAR_CALLBACKS_TRIPLE.items():
                Ta = start_time + choose_delay(dmin, dmax)
                enqueue_future_event(Ta, cb_name, Ccb, "Sonar", start_time)
        else:
            dmin, dmax, Ccb = SENSOR_CALLBACKS_SINGLE["Sonar"]
            Ta = start_time + choose_delay(dmin, dmax)
            enqueue_future_event(Ta, "Sonar", Ccb, "Sonar", start_time)

def pop_due_events(now: int):
    """Enqueue di tutti gli eventi con Ta==now."""
    if not future_events:
        return
    due = [e for e in future_events if e[0] == now]
    if not due:
        return
    future_events[:] = [e for e in future_events if e[0] != now]
    for Ta, cb_id, src, Ccb in due:
        aper_q.append({"id": cb_id, "src": src, "remaining": Ccb, "arrival": Ta, "C": Ccb})

while time < H:

    # 1) arrivi callback aperiodiche
    pop_due_events(time)

    # 2) release job periodici normali
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

    # 3) rilascio PS (job-based!)
    if time % Ts == 0:
        release_times[ps_name].append(time)
        # Crea job PS solo se c'è lavoro pendente ORA (Polling Server puro)
        if len(aper_q) > 0:
            ps_job = {"remaining": Cs, "period": Ts, "release": time}
        else:
            ps_job = None

    # 4) scelta RM tra:
    # - job periodico più prioritario (T min)
    # - PS job (se esiste) con periodo Ts
    ready_periodic = [j for j in jobs if j["remaining"] > 0]
    periodic_choice = min(ready_periodic, key=lambda j: j["period"]) if ready_periodic else None

    ps_ready = (ps_job is not None and ps_job["remaining"] > 0 and len(aper_q) > 0)

    run_ps = False
    if ps_ready:
        if periodic_choice is None:
            run_ps = True
        else:
            if Ts < periodic_choice["period"]:
                run_ps = True
            elif Ts == periodic_choice["period"]:
                run_ps = (not PERIODIC_WINS_TIE)
            else:
                run_ps = False

    # 5) esecuzione di 1ms
    if run_ps:
        ps_job["remaining"] -= 1

        # 1ms di servizio all'aperiodico in testa
        aper_q[0]["remaining"] -= 1
        ps_exec_src[time] = aper_q[0]["src"]
        schedule.append((time, ps_name))

        if aper_q[0]["remaining"] == 0:
            aper_q.popleft()

        # Se non c'è più nulla da servire, il job PS termina SUBITO (come il tuo thread)
        if len(aper_q) == 0:
            ps_job = None

        # Se finisce budget, termina
        if ps_job is not None and ps_job["remaining"] == 0:
            ps_job = None

    elif periodic_choice is not None:
        # start job => genera callback una sola volta
        if periodic_choice["id"] not in started_job_ids:
            started_job_ids.add(periodic_choice["id"])
            schedule_callbacks_from_task(periodic_choice["name"], time)

        periodic_choice["remaining"] -= 1
        schedule.append((time, periodic_choice["name"]))

        if periodic_choice["remaining"] == 0:
            # end-cap all'ultimo ms eseguito = time
            finish_caps.append((time, periodic_choice["name"]))

    else:
        schedule.append((time, "IDLE"))

    # pulizia job completati
    jobs = [j for j in jobs if j["remaining"] > 0]
    time += 1


# ============================================================
# PLOT
# ============================================================

fig, ax = plt.subplots(figsize=(18, 6))

# ordine RM (PS incluso): periodo crescente, PS sotto a parità
period_of = {name: T for (name, _, T) in tasks}
period_of[ps_name] = Ts

present = sorted({task for _, task in schedule if task != "IDLE"})
known = [t for t in present if t in period_of]
unknown = [t for t in present if t not in period_of]

known_sorted = sorted(known, key=lambda n: (period_of[n], 1 if n == ps_name else 0))
task_names = known_sorted + unknown

# layout
lane = 1.35
bar_h = 0.65
bar_offset = 0.0
N = len(task_names)

y_line = {name: (N - 1 - i) * lane for i, name in enumerate(task_names)}

# ============================================================
# PALETTE COLORI COERENTE (catena funzionale)
# ============================================================

# Colori principali (task periodici)
COLOR_CONTROLLER = "tab:blue"
COLOR_GYRO       = "tab:green"
COLOR_SONAR      = "tab:purple"
COLOR_COMM       = "tab:red"
COLOR_PS_FALLBACK = "tab:gray"

# Colori sonar distinti (aperiodici)
COLOR_SONAR_LEFT  = "#00C853"   # verde acceso (emerald)
COLOR_SONAR_FRONT = "#FF6D00"   # arancione acceso
COLOR_SONAR_RIGHT = "#D500F9"   # magenta acceso

COLOR_SONAR_LEFT  = "#00FF00"   # verde puro
COLOR_SONAR_FRONT = "#FF0000"   # rosso puro
COLOR_SONAR_RIGHT = "#0000FF"   # blu puro

COLOR_SONAR_LEFT  = "#E69F00"   # orange
COLOR_SONAR_FRONT = "#56B4E9"   # sky blue
COLOR_SONAR_RIGHT = "#CC79A7"   # reddish purple


# --- task periodici ---
task_color_map = {
    "Controller": COLOR_CONTROLLER,
    "Gyro":       COLOR_GYRO,
    "Sonar":      COLOR_SONAR,
    "Comm":       COLOR_COMM,
    "PS":         COLOR_PS_FALLBACK,  # usato solo se serve fallback
}

# --- callback / PS (stesso colore della sorgente logica) ---
src_color = {
    "Controller": COLOR_CONTROLLER,
    "Gyro":       COLOR_GYRO,

    # sonar triplo
    "SonarLeft":  COLOR_SONAR_LEFT,
    "SonarFront": COLOR_SONAR_FRONT,
    "SonarRight": COLOR_SONAR_RIGHT,

    # sonar singolo (se SONAR_TRIPLE_CALLBACKS = False)
    "Sonar":      COLOR_SONAR,
}

ps_fallback_color = COLOR_PS_FALLBACK

# 1) baseline
for name in task_names:
    ax.hlines(y_line[name], 0, H, colors="0.65", linewidth=1.0)

# 2) barre esecuzione
for t, task in schedule:
    if task == "IDLE":
        continue
    if task not in y_line:
        continue
    y = y_line[task]

    if task == ps_name:
        src = ps_exec_src.get(t, None)
        c = src_color.get(src, ps_fallback_color)
        ax.broken_barh([(t, 1)], (y + bar_offset, bar_h), facecolors=c, edgecolors="none")
    else:
        ax.broken_barh([(t, 1)], (y + bar_offset, bar_h), facecolors=task_color_map[task], edgecolors="none")

# 3) release
for name in task_names:
    xs = release_times.get(name, [])
    if xs:
        y = y_line[name]
        ax.vlines(xs, y, y + bar_h + 0.20, colors="black", linewidth=1.3)

# 4) end-cap fine job periodici (overlay)
for t_last, name in finish_caps:
    if name not in y_line:
        continue
    y = y_line[name]

    cap_h = bar_h
    cap_color = task_color_map.get(name, "black")

    if FINISH_CAP_MODE in ("tall", "both"):
        cap_h = max(bar_h, min(FINISH_CAP_HEIGHT, lane - 0.05))
    #if FINISH_CAP_MODE in ("color", "both"):
    #   cap_color = FINISH_CAP_COLOR

    ax.broken_barh([(t_last, 1)], (y + bar_offset, cap_h), facecolors=cap_color, edgecolors="none", zorder=10)

# 5) segmenti callback start→Ta (nella corsia del task periodico sorgente)
#    cb_links: (src_cb, task_src, t_start, Ta, C, cb_id)
for src_cb, task_src, t_start, Ta, Ccb, cb_id in cb_links:
    if task_src not in y_line:
        continue
    y = y_line[task_src]
    y_seg = y - 0.22

    # colore del segmento = task periodico sorgente
    if task_src == "Sonar":
        seg_color = task_color_map["Sonar"]   # viola
    else:
        seg_color = task_color_map.get(task_src, "black")

    ax.hlines(y_seg, t_start, Ta, colors=seg_color, linewidth=1.8, alpha=0.95)
    # stanghetta verticale = colore del callback specifico
    cb_color = src_color.get(src_cb, seg_color)

    ax.vlines(Ta, y_seg - 0.08, y_seg + 0.08, colors=cb_color, linewidth=2.0)

    mid = (t_start + Ta) / 2.0
    ax.text(mid, y_seg - 0.12, f"{src_cb}  C={Ccb}",
            ha="center", va="top", fontsize=7,
            bbox=dict(boxstyle="round,pad=0.12", fc="white", ec="0.5", lw=0.5),
            color="black")

# assi
ax.set_xlim(0, H)
ax.set_xlabel("Tempo [ms]")

mode = "worst-case" if USE_WORST_CASE_DELAY else f"random(seed={RANDOM_SEED})"
sonar_mode = "triplo" if SONAR_TRIPLE_CALLBACKS else "singolo"
ax.set_title(
    f"RM + Polling Server (puro, job-based) + callback dipendenti ({mode}, sonar {sonar_mode}) | "
    "PS colorato per callback | callback: segmento start→Ta"
)

ax.set_yticks([y_line[n] for n in task_names])
ax.set_yticklabels(task_names)

# ticks
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
ax.tick_params(axis="x", which="minor", bottom=False, labelbottom=False)

# griglie
ax.grid(True, which="minor", axis="x", linestyle="-", linewidth=0.10, color="0.90")
ax.grid(True, which="major", axis="x", linestyle="-", linewidth=0.55, color="0.78")
ax.grid(True, which="major", axis="y", linestyle="--", linewidth=0.35, color="0.82")

# ============================================================
# LEGENDA (basso a destra)
# ============================================================

legend_handles = []

# periodici: patch
for name in task_names:
    if name == ps_name:
        continue
    legend_handles.append(Patch(facecolor=task_color_map[name], edgecolor="none", label=f"{name} (periodico)"))

# PS: patch per callback servita
for src in CALLBACK_SOURCES:
    legend_handles.append(Patch(facecolor=src_color[src], edgecolor="none", label=f"PS serve {src}"))

# segmenti callback
for src in CALLBACK_SOURCES:
    # segmento di sonar triplo è viola (task Sonar)
    if src.startswith("Sonar") and SONAR_TRIPLE_CALLBACKS:
        seg_col = task_color_map["Sonar"]
    else:
        seg_col = src_color[src]
    legend_handles.append(Line2D([0], [0], color=seg_col, lw=2.0, label=f"Callback {src}: start→Ta"))

# release
legend_handles.append(Line2D([0], [0], color="black", lw=1.3, label="Release (|)"))

# end-cap
#legend_handles.append(Patch(facecolor=FINISH_CAP_COLOR if FINISH_CAP_MODE in ("color", "both") else "black",
#                            edgecolor="none", label="Fine job (end-cap)"))

ax.legend(handles=legend_handles, loc="lower right", fontsize=8, framealpha=0.95)

plt.tight_layout()
plt.show()