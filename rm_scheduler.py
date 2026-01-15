import matplotlib.pyplot as plt
from math import gcd
from functools import reduce

# -------------------------
# Task set: (name, C, T)  [tutti in ms]
# -------------------------
tasksB1 = [
    ("Batteria", 1, 200),
    ("Temperatura", 1, 200),
    ("Motore", 5, 20),
    ("Comm", 12, 50)
]

tasksB2 = [
    ("Controller", 2, 20),
    ("Gyro", 4, 40),
    ("Sonar", 25, 100),
    ("Comm", 12, 50)
]

tasksB2_Degr = [
    ("Controller", 2, 20),
    ("Gyro", 4, 40),
    ("Sonar", 25, 100),

    ("Motore", 5, 20)
]

# CAMBIARE
tasks = tasksB1

# -------------------------
# Utility functions
# -------------------------
def lcm(a, b):
    return a * b // gcd(a, b)

def hyperperiod(periods):
    return reduce(lcm, periods)

# -------------------------
# Rate Monotonic ordering (priorità: periodo più piccolo)
# -------------------------
tasks = sorted(tasks, key=lambda x: x[2])
task_names = [t[0] for t in tasks]

# -------------------------
# Schedulability test (Liu & Layland) - sufficiente
# -------------------------
U = sum(C / T for _, C, T in tasks)
n = len(tasks)
U_ll = n * (2**(1/n) - 1)

print(f"Utilizzazione totale U = {U:.3f} ({U*100:.1f}%)")
print(f"Limite teorico RM = {U_ll:.3f} ({U_ll*100:.1f}%)")
print("✔ Test sufficiente: schedulabile" if U <= U_ll else "✘ Test non garantisce schedulabilità")

# -------------------------
# Simulation parameters
# -------------------------
H = hyperperiod([t[2] for t in tasks])   # iperperiodo (ms)
time = 0

schedule = []          # (t, running_task) ogni 1 ms
jobs = []              # lista job attivi

release_times = {name: [] for name, _, _ in tasks}   # rientro nello scheduler (release)
finish_marks = []      # lista di tuple (finish_time, task_name) per mettere la X

job_id_counter = 0

# -------------------------
# RM simulation (time-driven, step=1 ms)
# -------------------------
while time < H:

    # Release new jobs
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

    # Select highest priority ready job
    ready = [j for j in jobs if j["remaining"] > 0]

    if ready:
        current = min(ready, key=lambda j: j["period"])
        current["remaining"] -= 1
        schedule.append((time, current["name"]))

        # se ha finito esattamente ora (dopo aver consumato questo ms)
        if current["remaining"] == 0:
            finish_time = time + 1   # fine dello slot
            finish_marks.append((finish_time, current["name"]))
    else:
        schedule.append((time, "IDLE"))

    # Remove completed jobs
    jobs = [j for j in jobs if j["remaining"] > 0]

    time += 1

# -------------------------
# Plot scheduling (Gantt-like)
# -------------------------
fig, ax = plt.subplots(figsize=(14, 4))

# priorità alta in alto
task_names = [t[0] for t in tasks]
y_pos = {name: i for i, name in enumerate(task_names[::-1])}

# colori task
colors = plt.cm.tab10(range(len(task_names)))
color_map = {name: colors[i] for i, name in enumerate(task_names)}

bar_height = 0.8

# rettangoli esecuzione
for t, task in schedule:
    if task != "IDLE":
        ax.broken_barh([(t, 1)], (y_pos[task], bar_height),
                       facecolors=color_map[task])

# --- barre nere di inizio (release) ---
release_bar_extra = 0.25  # quanto sporge sopra la barra del task
for name in task_names:
    xs = release_times[name]
    if xs:
        ax.vlines(
            xs,
            ymin=y_pos[name],
            ymax=y_pos[name] + bar_height + release_bar_extra,
            colors='black',
            linewidth=2.0
        )

# --- X nere di completamento job ---
for ft, name in finish_marks:
    ax.plot(
        ft,
        y_pos[name] + bar_height + release_bar_extra * 0.6,
        marker='x',
        color='black',
        markersize=6,
        linestyle='None'
    )

# Asse Y
ax.set_yticks(list(y_pos.values()))
ax.set_yticklabels(task_names[::-1])
ax.set_xlim(0, H)
ax.set_xlabel("Tempo [ms]")
ax.set_title("RM Scheduling: release (barra nera) e fine job (X nera)")

# --- TICK LEGIBILI (pochi) ---
if H <= 200:
    tick_step = 10
elif H <= 1000:
    tick_step = 50
elif H <= 5000:
    tick_step = 250
else:
    tick_step = 500

ax.set_xticks(range(0, H + 1, tick_step))

# --- GRIGLIA A SLOT (minor) senza label ---
ax.set_xticks(range(0, H + 1, 1), minor=True)
ax.tick_params(axis='x', which='minor', bottom=False, labelbottom=False)
ax.tick_params(axis='x', which='major', labelsize=9)

ax.grid(True, which='minor', axis='x', linestyle='-', linewidth=0.2)
ax.grid(True, which='major', axis='x', linestyle='-', linewidth=0.7)
ax.grid(True, which='major', axis='y', linestyle='--', linewidth=0.5)

plt.tight_layout()
plt.show()
