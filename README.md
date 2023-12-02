## Unconstrained Weakly Submodular minus Modular (UWSM) Maximization Problem

Find a subset of $V$ such that:
$$\arg \max_{S \subseteq V} ~ f(S)-c(S)$$
where $f$ is monotone (weakly) submodular and $c$ is modular

We propose an algorithm solving the above problem with provable theoretical guarantee:
$$f(S)-c(S) \geq \alpha(1-\epsilon)f(OPT)-c(OPT)-\frac{c(OPT)}{\alpha(1-\epsilon)}\log\frac{f(OPT)}{c(OPT)}$$

The running time is ${\mathcal{O}}(\frac{n}{\epsilon}\log \frac{n}{\alpha \epsilon})$, where $\alpha \in (0,1]$ is the submodularity ratio.

A set function $f:2^V \rightarrow \mathbb{R}^{\geq 0}$ is $\alpha$-weakly submodular if for any set $S \subseteq T \subseteq V$:
$$\sum\nolimits_{u\in T \setminus S} f(u \mid S)  \geq  \alpha \big(f(T)-f(S)\big)$$

Note that $f$ is submodular iff $\alpha=1$.
