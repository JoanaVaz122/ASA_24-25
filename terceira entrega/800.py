import pulp
import sys

def solve_toy_distribution(input_data):
    lines = input_data.strip().split('\n')
    n, m, t = map(int, lines[0].strip().split())

    factories = []
    for i in range(1, n + 1):
        factories.append(tuple(map(int, lines[i].strip().split())))

    countries = []
    for i in range(n + 1, n + m + 1):
        countries.append(tuple(map(int, lines[i].strip().split())))

    requests = []
    for i in range(n + m + 1, n + m + t + 1):
        requests.append(list(map(int, lines[i].strip().split())))

    problem = pulp.LpProblem("MaximizeToyDistribution", pulp.LpMaximize)

    # Decision variables for each child-factory pair
    x = {}
    for req in requests:
        child_id = req[0]
        factories_list = req[2:]
        for factory_id in factories_list:
            x[(child_id, factory_id)] = pulp.LpVariable(f"x_{child_id}_{factory_id}", cat="Binary")

    # Objective: Maximize the number of satisfied requests
    problem += pulp.lpSum(x.values()), "Maximize total satisfied requests"

    # Factory usage constraints
    factory_usage = {i: pulp.lpSum(0) for i in range(1, n + 1)}
    for (child_id, factory_id), var in x.items():
        factory_usage[factory_id] += var

    for f_id, _, f_max in factories:
        problem += factory_usage[f_id] <= f_max, f"StockConstraint_factory_{f_id}"

    # Each child can receive at most one toy, from one factory
    for req in requests:
        child_id = req[0]
        factories_list = req[2:]
        problem += pulp.lpSum(x[(child_id, factory_id)] for factory_id in factories_list) <= 1, f"OneToyPerChild_{child_id}"

    # Country export constraints
    country_exports = {j: pulp.lpSum(0) for j in range(1, m + 1)}
    for (child_id, factory_id), var in x.items():
        child_country = next(req[1] for req in requests if req[0] == child_id)
        country_exports[child_country] += var

    for country_id, p_max, p_min in countries:
        problem += country_exports[country_id] <= p_max, f"ExportConstraint_country_{country_id}"
        problem += country_exports[country_id] >= p_min, f"MinCountryConstraint_{country_id}"

    solver = pulp.PULP_CBC_CMD(msg=False)
    problem.solve(solver)

    if pulp.LpStatus[problem.status] != "Optimal":
        print(-1)
        return

    print(int(pulp.value(problem.objective)))

if __name__ == "__main__":
    input_data = sys.stdin.read()
    solve_toy_distribution(input_data)
