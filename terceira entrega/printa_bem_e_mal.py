from pulp import *
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

    problem = LpProblem("MaximizarDistribuicaoBrinquedos", LpMaximize)

    # Variáveis de decisão para cada par criança-fábrica
    x = {}
    for req in requests:
        child_id = req[0]
        factories_list = req[2:]
        for factory_id in factories_list:
            x[(child_id, factory_id)] = LpVariable(f"x_{child_id}_{factory_id}", cat="Binary")

    # Objetivo: Maximizar o número de pedidos satisfeitos
    problem += lpSum(x.values()), "Maximizar total de pedidos satisfeitos"

    # Restrições de uso das fábricas
    factory_usage = {i: lpSum(0) for i in range(1, n + 1)}
    for (child_id, factory_id), var in x.items():
        factory_usage[factory_id] += var

    for f_id, _, f_max in factories:
        problem += factory_usage[f_id] <= f_max, f"RestricaoEstoque_fábrica_{f_id}"

    # Cada criança pode receber no máximo um brinquedo, de uma fábrica
    for req in requests:  
        child_id = req[0]
        factories_list = req[2:]
        problem += lpSum(x[(child_id, factory_id)] for factory_id in factories_list) <= 1, f"UmBrinquedoPorCrianca_{child_id}"

    # Restrições de exportação dos países
    country_exports = {j: lpSum(0) for j in range(1, m + 1)}
    for (child_id, factory_id), var in x.items():
        child_country = next(req[1] for req in requests if req[0] == child_id)
        country_exports[child_country] += var

    for country_id, p_max, p_min in countries:
        problem += country_exports[country_id] <= p_max, f"RestricaoExportacao_pais_{country_id}"
        problem += country_exports[country_id] >= p_min, f"RestricaoMinimaPais_{country_id}"

    solver = PULP_CBC_CMD(msg=False)
    problem.solve(solver)
     
    print(int(problem.objective.value()))

    # Verifica se o problema é inviável
    if LpStatus[problem.status] == "Infeasible" or LpStatus[problem.status] != "Optimal":
        print(-1) 
    else:
        print(int(problem.objective.value()))

if __name__ == "__main__":
    input_data = sys.stdin.read()
    solve_toy_distribution(input_data)
