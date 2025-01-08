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

    x = []
    for req in requests:
        x.append(pulp.LpVariable(f"x_{req[0]}", cat="Binary"))

    problem += pulp.lpSum(x), "Maximize total satisfied requests"

    factory_usage = {i: pulp.lpSum(0) for i in range(1, n + 1)}
    for var, req in zip(x, requests):
        for factory in req[2:]:
            factory_usage[factory] += var

    for f_id, country_id, f_max in factories:
        problem += factory_usage[f_id] <= f_max, f"StockConstraint_factory_{f_id}"

    country_exports = {j: pulp.lpSum(0) for j in range(1, m + 1)}
    for var, req in zip(x, requests):
        country_exports[req[1]] += var

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
