# Code can be run simply as Python script:
# python3 main.py

import numpy as np

# Constants for program
DIMENSION = 20
EPS = 1e-6
TYPE_GAUSS = "Gauss"
TYPE_JACOBI = "Jacobi"


# Function to generate A matrix based on gamma
def gen_a(gamma):
    A = np.zeros((DIMENSION, DIMENSION))  # float64 (double precision is default)
    assert (type(A[0][0]) == np.float64)  # check that double precision is really default
    for i in range(DIMENSION):
        if i > 0:
            A[i][i - 1] = -1
        if i < DIMENSION - 1:
            A[i][i + 1] = -1
        A[i][i] = gamma
    return A


# Function to generate b vector based on gamma
def gen_b(gamma):
    b = np.zeros((DIMENSION, 1))  # float64 (double precision is default)
    for i in range(DIMENSION):
        if i == 0 or i == DIMENSION - 1:
            b[i] = gamma - 1
        else:
            b[i] = gamma - 2
    return b


# Function to generate starting x vector
def gen_x0():
    return np.zeros((DIMENSION, 1))  # float64 (double precision is default)


# Function to generate matrix Q (program parameter)
def gen_q(A, method):
    Q = np.zeros((DIMENSION, DIMENSION))
    for i in range(DIMENSION):
        for j in range(DIMENSION):
            if i == j or (method == TYPE_GAUSS and i > j):
                Q[i][j] = A[i][j]
    return Q


# Function to check, whether the requirement for convergence is met
def does_converge(W):
    eigenvalues = np.linalg.eigvals(W)
    spectralRadius = max(np.max(eigenvalues), -np.min(eigenvalues))
    print(f"Spectral radius: {spectralRadius}")
    return spectralRadius < 1


# Generates linear equation and tries to solve it by given method
def generate_solve(gamma, method):
    # Generate matrices
    A = gen_a(gamma)
    b = gen_b(gamma)
    x0 = gen_x0()

    # Generate matrix Q based on method
    Q = gen_q(A, method)
    Q_inv = np.linalg.inv(Q)

    # Calculate matrix W
    W = np.subtract(np.identity(A.shape[0]), np.matmul(Q_inv, A))

    # Check convergence
    if not does_converge(W):
        return None, None

    # Iteration method
    x_k = x0
    i = 0
    while (np.linalg.norm(np.subtract(np.matmul(A, x_k), b)) / np.linalg.norm(b)) >= EPS:
        x_k = np.matmul(Q_inv, np.add(np.matmul(np.subtract(Q, A), x_k), b))
        i += 1
    return i, x_k


# Wrapper function for solving and generating equations
def solve_with(gamma, method):
    iterations, solution = generate_solve(gamma, method)
    if iterations is None:
        print(f"Gamma {gamma} is non solvable with {method} (does not converge)")
    else:
        print(f"Gamma {gamma} by {method} finished with {iterations} iterations and result {np.transpose(solution)}")


# Runner
if __name__ == '__main__':
    assignment = [10, 2, 0.8]
    for gamma in assignment:
        solve_with(gamma, TYPE_JACOBI)
        solve_with(gamma, TYPE_GAUSS)
