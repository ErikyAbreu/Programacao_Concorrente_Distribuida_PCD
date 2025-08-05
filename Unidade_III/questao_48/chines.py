import numpy as np

data = {
    "p": np.array([1, 2, 4]),  # Número de processos
    "N": np.array([2147483648, 4294967296, 8589934592, 17179869184]),  # Número de trapézios
    "T": np.array([
        [6.0597, 12.5721, 20.8969, 42.6701],  # p = 1
        [3.6865, 7.2944, 11.5223, 23.4017],  # p = 2
        [2.2202, 4.5017, 6.6425, 13.3833],  # p = 4
    ])
}

X = []  # Features: [n/p, log2(p)]
y = []  # Target: tempo medido

for i, p in enumerate(data["p"]):
    for j, N in enumerate(data["N"]):
        x1 = N / p  # n/p
        x2 = np.log2(p)  # log2(p)
        X.append([x1, x2])
        y.append(data["T"][i, j])  # Tempo medido correspondente

X = np.array(X)  # Conversão para array numpy
y = np.array(y)

X = np.c_[X, np.ones(X.shape[0])]

coefficients = np.linalg.lstsq(X, y, rcond=None)[0]

a, b, c = coefficients

print("Coeficientes estimados:")
print(f"a (n/p)     = {a:.6e}")
print(f"b (log2(p)) = {b:.6e}")
print(f"c (bias)    = {c:.6e}")

print("\nComparação entre tempos medidos e estimados:")
print("n/p\t\tlog2(p)\t\tTempo Medido\tTempo Estimado")
for (n, p, time), (x1, x2) in zip([(N, p, t) for p, row in zip(data["p"], data["T"]) for N, t in zip(data["N"], row)], X[:, :-1]):
    tempo_estimado = a * x1 + b * x2 + c
    print(f"{x1:.2e}\t{x2:.2f}\t\t{time:.6f}\t{tempo_estimado:.6f}")