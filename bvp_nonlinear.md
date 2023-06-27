# BVP for Nonlinear Differential Equations

solve
$$y'' = \left[ x^2 (y')^2 -9y^2 + 4x^6\right]/x^5,\quad y(1) = 0,\quad y(2) = 8\ln 2.$$

Exact yution is $y(x) = x^3 \ln x$.

```python
import numpy as np
import matplotlib.pyplot as plt
```

## 1. Nonlinear Eqn. Solver (`scipy.optimize.fsolve`)

```python
# x cos(y) = 4, xy - y = 5
from scipy import optimize

# u = [x, y], u0 = [0.01, 0.01]
def eqn(u):
    x, y = u
    return x*np.cos(y) - 4, x*y - y - 5

optimize.fsolve(eqn, [0.01, 0.01])
```

```python
# FDM - Nonlinear eqn yver
from scipy import optimize

def exact(x): return np.log(x)*x**3
def f(x, y, dy): return (x**2 * dy**2 -9*y**2 + 4*x**6)/x**5

def eqns(y, x, h):
    xi = x[1: -1]
    yi = y[1: -1]
    dyi = (y[2:] - y[:-2]) / h/ 2
    d2yi = (y[:-2] -2*y[1:-1] + y[2:]) / h**2
    
    res = np.zeros_like(y)
    res[1:-1] = d2yi - f(xi, yi, dyi)   # i = 1, ..., n-1
    res[0] = y[0] - 0                   # i = 0 (BC)
    res[-1] = y[-1] - np.log(256)       # i = n (BC)
    return res

x, h = np.linspace(1, 2, 101, retstep=True)
y0 = np.ones_like(x)*0.1 # initial guess
y = optimize.fsolve(eqns, y0, args=(x, h))

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
ax1.plot(x, exact(x), 'k:', label="Exact")
ax1.plot(x, y, 'g', label="Numerical")
ax1.set_ylim(0, 6)

ax2.semilogy(x, abs(exact(x) - y), 'r', label="Error")
ax2.set_ylim(1e-6, 1e-3)

for ax in (ax1, ax2):
    ax.set_xlim(1, 2)
    ax.legend()
    ax.grid(ls=':', lw=1, c='k')

plt.show()
```

## 2. Shooting method

```python
from scipy import integrate, optimize

def exact(x): return np.log(x)*x**3
def f(x, y, dy): return (x**2 * dy**2 -9*y**2 + 4*x**6)/x**5

# u = [y, dy], u0 = [y0, dy0]
def eqn(x, u):
    y, dy = u
    return dy, f(x, y, dy)

def root(dy0, y0, yn):
    return yn - integrate.odeint(eqn, [y0, dy0], x, tfirst=True)[-1, 0]

y0, yn = 0, np.log(256)
x = np.linspace(1, 2, 101)
dy0 = optimize.newton(root, 0.1, args=(y0, yn))
y = integrate.odeint(eqn, [y0, dy0], x, tfirst=True)[:, 0]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
ax1.plot(x, exact(x), 'k:', label="Exact")
ax1.plot(x, y, 'g', label="Numerical")
ax1.set_ylim(0, 6)

ax2.semilogy(x, abs(exact(x) - y), 'r', label="Error")
ax2.set_ylim(1e-10, 1e-6)

for ax in (ax1, ax2):
    ax.set_xlim(1, 2)
    ax.legend()
    ax.grid(ls=':', lw=1, c='k')

plt.show()
```

```python
# bisect and RK5
def bisect(func, a, b, args=(), maxiter=1000, tol=2e-12):
    fa = func(a, *args)
    fb = func(b, *args)

    for k in range(maxiter):
        c = 0.5*(a + b)
        fc = func(c, *args)

        if abs(b - a) < tol: break
        elif fb*fc < 0: a, fa = c, fc
        else:           b, fb = c, fc

    return c

def rk4(f, u0, t, args=()):
    u = np.asfarray([u0]*t.size)
    h = (t[-1] - t[0])/float(t.size - 1)

    for i in range(t.size - 1):
        k1 = h*f(t[i], u[i], *args)
        k2 = h*f(t[i] + h/2, u[i] + k1/2, *args)
        k3 = h*f(t[i] + h/2, u[i] + k2/2, *args)
        k4 = h*f(t[i] + h, u[i] + k3, *args)
        u[i+1] = u[i] + (k1 + 2*k2 + 2*k3 + k4)/6
    return u
```

```python
# bisect + rk4
def exact(x): return np.log(x)*x**3
def f(x, y, dy): return (x**2 * dy**2 -9*y**2 + 4*x**6)/x**5

# u = [y, dy], u0 = [y0, dy0]
def eqn(x, u):
    y, dy = u
    return np.array([dy, f(x, y, dy)])

def root(dy0, y0, yn):
    return yn - rk4(eqn, [y0, dy0], x)[-1, 0]

y0, yn = 0, np.log(256)
x = np.linspace(1, 2, 101)
dy0 = bisect(root, 0.1, 1.5, args=(y0, yn))   # dy0 = 1.0
y = rk4(eqn, [y0, dy0], x)[:, 0]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
ax1.plot(x, exact(x), 'k:', label="Exact")
ax1.plot(x, y, 'g', label="Numerical")
ax1.set_ylim(0, 6)

ax2.semilogy(x, abs(exact(x) - y), 'r', label="Error")
# ax2.set_ylim(1e-10, 1e-6)

for ax in (ax1, ax2):
    ax.set_xlim(1, 2)
    ax.legend()
    ax.grid(ls=':', lw=1, c='k')

plt.show()
```

## 3. Newton method - 1

```python
from scipy.sparse import spdiags
from scipy.sparse.linalg import spsolve

def solve_tdm(x, coeffs, bc_0, bc_n):
    """ Tridiagonal matrix yver for a BVP
        x = np.linspace(x0, xn, n + 1)
        coeffs = [a, b, c, d]
        bc_0 = [p0, q0, r0] <-- p0*y0 + q0*y'0 = r0
        bc_n = [pn, qn, rn] <-- pn*yn + qn*y'n = rn
    """

    ## Tridiagonal matrix:
    h = (x[-1] - x[0])/float(x.size - 1)
    a, b, c, d = map(np.asfarray, coeffs)
    diags = np.r_[a[1:-1], 0, 0], np.r_[0, b[1:-1], 0], np.r_[0, 0, c[1:-1]]
    A = spdiags(diags, [-1, 0, 1], x.size, x.size, format='lil')

    ## BC at x = x0:
    p0, q0, r0 = bc_0
    A[0, :3] = 2*h*p0 - 3*q0, 4*q0, -q0
    d[0] = 2*r0*h

    ## BC at x = xn:
    pn, qn, rn = bc_n
    A[-1, -3:] = qn, -4*qn, 2*pn*h + 3*qn
    d[-1] = 2*rn*h

    return spsolve(A.tocsr(), d)
```

```python
def exact(x): return np.log(x)*x**3

def f(x, y, dy): return (x**2 * dy**2 -9*y**2 + 4*x**6)/x**5
def dfdy(x, y, dy): return -18*y/x**5
def dfddy(x, y, dy): return 2*dy/x**3

def update(y, x, h):
    dy = np.zeros_like(y)
    dy[1:-1] = (y[2:] - y[:-2])/h/2
    
    a = 1 + dfddy(x, y, dy)*h/2
    b = -2 - dfdy(x, y, dy)*h**2
    c = 1 - dfddy(x, y, dy)*h/2
    d = (f(x, y, dy) - dfdy(x, y, dy)*y - dfddy(x, y, dy)*dy)*h**2

    return solve_tdm(x, [a, b, c, d], (1, 0, 0), (1, 0, np.log(256)))

x, h = np.linspace(1, 2, 101, retstep=True)
y0 = np.ones_like(x)*0.01

for k in range(100):
    y = update(y0, x, h)
    if np.allclose(y, y0): break
    else: y0 = y

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
ax1.plot(x, exact(x), 'k:', label="Exact")
ax1.plot(x, y, 'g', label="Numerical")
ax1.set_ylim(0, 6)

ax2.semilogy(x, abs(exact(x) - y), 'r', label="Error")
ax2.set_ylim(1e-6, 1e-3)

for ax in (ax1, ax2):
    ax.set_xlim(1, 2)
    ax.legend()
    ax.grid(ls=':', lw=1, c='k')

plt.show()
```

## 3. Newton method 2

```python
def exact(x): return np.log(x)*x**3

def F(x, ym, y, yp): return (x**2 * ((yp - ym)/h/2)**2 -9*y**2 + 4*x**6)/x**5
def dFdy(x, ym, y, yp): return -18*y/x**5
def dFdym(x, ym, y, yp): return -(yp - ym)/2/h**2/ x**3
def dFdyp(x, ym, y, yp): return +(yp - ym)/2/h**2/ x**3

def update(y, x, h):
    yp = np.ones_like(y)
    yp[1:-1] = y[2:]
    ym = np.ones_like(y)
    ym[1:-1] = y[:-2]
    
    a = 1 - dFdym(x, ym, y, yp)*h**2
    b = -2 - dFdy(x, ym, y, yp)*h**2
    c = 1 - dFdyp(x, ym, y, yp)*h**2
    d = (F(x, ym, y, yp) - dFdym(x, ym, y, yp)*ym - dFdy(x, ym, y, yp)*y - dFdyp(x, ym, y, yp)*yp)*h**2

    return solve_tdm(x, [a, b, c, d], (1, 0, 0), (1, 0, np.log(256)))

x, h = np.linspace(1, 2, 101, retstep=True)
y0 = np.ones_like(x)*0.01

for k in range(100):
    y = update(y0, x, h)
    if np.allclose(y, y0): break
    else: y0 = y

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 4))
ax1.plot(x, exact(x), 'k:', label="Exact")
ax1.plot(x, y, 'g', label="Numerical")
ax1.set_ylim(0, 6)

ax2.semilogy(x, abs(exact(x) - y), 'r', label="Error")
ax2.set_ylim(1e-6, 1e-3)

for ax in (ax1, ax2):
    ax.set_xlim(1, 2)
    ax.legend()
    ax.grid(ls=':', lw=1, c='k')

plt.show()
```
