# Analysis and Design of Algorithms Project

This project was carried out during the Algorithm Analysis and Design course at UFPB, taught by Professor Dr. Bruno Bruck.

## Problem Description

Shortly after graduating from college, Ana decided to become an entrepreneur and teamed up with her friend Paula to start a consultancy in the field of Operations Research. The company primarily focuses on solving combinatorial optimization problems for businesses across various sectors, using both exact algorithms and heuristics.

After three difficult and challenging years, the company is now experiencing a great phase and has been growing significantly in recent months with the acquisition of important clients. Their business model primarily focuses on licensing web solutions developed in-house, so the client pays a much lower initial fee, only to finance part of the development and customization, and then can freely use the solution by paying an annual subscription. Ana attributes much of the company's success to this model. However, it does create some complications. In particular, the execution of the optimization algorithms themselves consumes most of the company's computational resources. Every time a client accesses the web application and requests a simulation, a job is generated for the execution of the respective algorithm. This job must be allocated and executed on one of the company's servers.

So far, all operational applications run on local servers, acquired and maintained by the company itself. However, during the last strategic planning meeting, it was conveyed to Ana and Paula that, to keep up with the demand, a significant investment would be necessary to update and scale the infrastructure. One alternative presented was to keep the application hosting local but outsource the job processing to cloud servers. In this case, the company only pays for the processing time consumed on the contracted server.

The operation of the new system was planned as follows. When a customer submits a request to run a simulation, a job is created and added to a queue. Several times a day, the company's system collects all the jobs in the queue and distributes them among cloud servers. Since Ana and Paula's company specializes in Optimization, they decided to develop an algorithm to determine the best allocation of jobs among the available servers. To better understand the problem and its requirements, consider the following formal definition:

> Let $n$ be the number of jobs and $m$ the number of cloud servers available in the company's portfolio. Each job $j \in {1, \ldots, n}$ requires a processing time $t_{sj}$ to be processed on server $s \in {1, \ldots, m}$ and incurs a cost $c_{sj}$. Additionally, each server $s$ has a capacity $b_s$, which specifies the maximum amount of time that can be contracted. For each job not allocated to a cloud server, we will assume it will be run locally at a fixed cost $p$. Therefore, the objective of the problem is to allocate each job to one of the servers without exceeding their capacity and to minimize the total cost for the company.

## Example Solution and Instance

Para exemplificar o problema, considere uma instância (cenário) onde $n = 6$, $m= 2$ e $p = 1000$. O vetor de capacidades do servidor é dado por $b = {220, 350}$, e as matrizes $t$ e $c$ são as seguintes:

The table below is the **t matrix**

|       | $j_1$ | $j_2$ | $j_3$ | $j_4$ | $j_5$ | $j_6$ |
|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
|server 1|  120  |  80  |  180  |   95  |   35  |   52  |   
|server 2|  145  |  70  |  230  |   70  |   40  |   59  |

The table below is the **c matrix**

|       | $j_1$ | $j_2$ | $j_3$ | $j_4$ | $j_5$ | $j_6$ |
|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|:-----:|
|server 1|  350  |  50  |  540  |   245  |   145  |   200  |   
|server 2|  410  |  80  |  500  |   200  |   100  |   196  |


In this case, a possible solution would be as follows:

![apa_solution](https://github.com/ryann-arruda/apa-project/assets/53544629/56f02020-0a11-4a10-971e-8069c1710bc1)

## Implemented Algorithms

### Greedy Algorithm

This implemented algorithm seeks to generate an initial solution for Local Search and Metaheuristic Algorithms.

The policy used to choose candidates (jobs) was as follows: jobs were selected considering their time, prioritizing those with the shortest time, distributing them across servers so that they did not exceed their capabilities.

The objective of this idea was to minimize the cost of jobs allocated on the local server, as it is the one with the highest allocation cost, filling the cloud servers as much as possible.

### Neighborhood Structures

An important part of the code for Local Search are Neighborhood Structures, which seek to generate new solutions (neighborhoods) from the current solution, causing the search algorithm to escape local optima, improve the value of the solution and get closer to or find the global optimum.

The Neighborhood Structures chosen were:

1. **Swap**: This neighborhood structure runs throughout the solution, checking whether it is possible to perform an exchange movement between two jobs on different servers. This exchange can occur between a cloud server and another cloud server, or between a cloud server and a local server.

2. **Reinsertion**: This movement seeks to remove a job from a server and place it on another server, if it has space to execute that job. This neighborhood structure can occur between a cloud server and another cloud server, between a cloud server and a local server, or between a local server and a cloud server.

3. **Block Reinsertion**: This neighborhood movement seeks to remove a block of two jobs from one cloud server to another cloud server, if it has space available.

An important observation that the Professor made was that all structures in the neighborhood should carry out an exhaustive search, that is, go through all possibilities for movement.

### Local Search Algorithm

The **Local Search Algorithm** requested by the Professor to be implemented was VND (Variable Neighborhood Descent). It explores the solution space through systematic neighborhood exchanges.

The pseudocode of this algorithm can be viewed below.
$$
\begin{aligned}
& \textbf{\text{procedure VND}}(f(.), N(.), r, s) \\
& \quad \text{Let } r \text{ be the number of different neighborhood structures;} \\
& \quad k \leftarrow 1; \quad \quad \quad \{ \text{Current neighborhood structure type} \} \\ \\
& \quad \underline{\text{while}}(k \leq r) \quad \underline{\text{do}} \\
& \quad \quad \text{Find the best neighbor} \quad s' \in N^{(k)}(s); \\
& \quad \quad \underline{\text{if}} (f(s') < f(s)) \\
& \quad \quad \quad \underline{\text{so}} \\
& \quad \quad \quad \quad s \leftarrow s'; \\
& \quad \quad \quad \quad k \leftarrow 1; \\
& \quad \quad \quad \underline{\text{else}} \\
& \quad \quad \quad \quad \quad k \leftarrow k + 1; \\
& \quad \quad \underline{\text{end-if}}; \\
& \quad \underline{\text{end-while}}; \\
& \quad \text{Return s} \\
& \text{\textbf{end} VND};
\end{aligned}
$$

### Metaheuristics

Other algorithms that the Professor suggested to be implemented were the GRASP or ILS metaheuristics. However, their implementations were optional.

Metaheuristic algorithms are capable of finding good solutions and, in some cases, the optimal solution. Unlike traditional heuristics, they are general in nature and can be applied to any optimization problem, in addition to having mechanisms to escape local optima.

The metaheuristic chosen to be implemented was ILS (Iterated Local Search). The general idea of ​​this algorithm is that a local search procedure can be improved by generating new initial solutions through perturbations of the local optimal solution.

The pseudocode of this algorithm can be viewed below.
$$
\begin{aligned}
& \textbf{\text{procedure ILS}} \\
& \quad s_0 \leftarrow \text{GenerateInitialSolution()}; \\
& \quad s \leftarrow \text{LocalSearch}(s_0); \\
& \quad \text{\underline{while}} \text{(the stopping criteria are not satisfied)} \quad \text{\underline{do}}\\
& \quad \quad s' \leftarrow \text{Perturbation(historic, s)}\\
& \quad \quad s'' \leftarrow \text{LocalSearch(s')};\\
& \quad \quad s \leftarrow \text{AcceptanceCriteria(s, s'', historic)}; \\
& \quad \text{\underline{end-while}} \\
& \quad \text{Return s} \\
& \text{\textbf{end} ILS};
\end{aligned}
$$

## Input File Format

The file format to be used is as follows:

```plaintext
n
m
p

b array

t matrix (servers x jobs)

c matrix (servers x jobs)
```

**NOTE:** The letters used in the format above follow the formal definition of the problem.

## Output File Format

The output format should be as follows.

```plaintext
<solution value generated by Local Search>
<server allocation cost>
<cost associated with jobs running locally>

<list of jobs executed on the first server>
<list of jobs executed on the second server>
...
<list of jobs executed on the m server>
```

## Requested Comparison Table

The Professor requested a table with the results obtained by the constructive heuristics, VND and metaheuristics (if implemented). This table must contain the following values:

* Best solution found for each input instance;
* Average time taken by the respective algorithm to find the solution for each input instance;
* GAP for the optimal solution for each input instance;

**NOTE:** The Professor advised that, if there is any random factor in the implemented algorithms, the average value of the solution, execution time and GAP metric of at least 10 executions for each instance must be included.

### GAP Calculation

The GAP value indicates the “distance” to the solution, in the case of constructive heuristics, for the optimal value. Additionally, this metric is given as a percentage (%).

$$gap = \left(\frac{value_{heuristic} - value_{optimal}}{value_{optimal}}\right) \times 100$$

Where,

* $value_{heuristic}$: value found by the heuristic for this instance;
* $value_{optimal}$: optimal value for this instance.

### Results Obtained

![table](https://github.com/ryann-arruda/apa-project/assets/53544629/d1300c99-900c-4d0c-bc5d-bb05682967d4)

## Authors

* Alexandre Bezerra ([Alexandreprog](https://github.com/Alexandreprog))
* Ryann Carlos ([ryann-arruda](https://github.com/ryann-arruda))

## Note

This README was produced through adaptations of the project specification provided by the Professor of the subject, as well as through materials also made available by him and class notes from the subject.