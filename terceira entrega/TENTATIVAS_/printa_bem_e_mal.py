from pulp import LpProblem, LpMaximize, LpVariable, lpSum, PULP_CBC_CMD, LpStatusOptimal
import sys

def solve_toy_distribution(input_data):
    lines = input_data.strip().split('\n')
    n, m, t = map(int, lines[0].strip().split())

    # Informações das fábricas
    factories = {}
    for i in range(1, n + 1):
        factory_id, country_id, f_max = map(int, lines[i].strip().split())
        factories[factory_id] = (country_id, f_max)

    # Informações dos países
    countries = {}
    for i in range(n + 1, n + m + 1):
        country_id, p_max, p_min = map(int, lines[i].strip().split())
        countries[country_id] = (p_max, p_min)

    # Informações dos pedidos
    requests = []
    for i in range(n + m + 1, n + m + t + 1):
        req = list(map(int, lines[i].strip().split()))
        requests.append(req)

    # Modelo de otimização
    problem = LpProblem("MaximizarDistribuicaoBrinquedos", LpMaximize)

    # Variáveis de decisão
    x = {}
    for req in requests:
        child_id = req[0]
        available_factories = req[2:]
        for factory_id in available_factories:
            x[(child_id, factory_id)] = LpVariable(f"x_{child_id}_{factory_id}", cat="Binary")

    # Objetivo: Maximizar pedidos satisfeitos
    problem += lpSum(x.values()), "Maximizar total de pedidos satisfeitos"

    # Restrições de uso das fábricas
    factory_usage = {f_id: lpSum(0) for f_id in factories.keys()}
    for (child_id, factory_id), var in x.items():
        factory_usage[factory_id] += var

    for factory_id, (_, f_max) in factories.items():
        problem += factory_usage[factory_id] <= f_max, f"RestricaoEstoque_fabrica_{factory_id}"

    # Cada criança pode receber no máximo um brinquedo
    for req in requests:
        child_id = req[0]
        available_factories = req[2:]
        problem += lpSum(x[(child_id, factory_id)] for factory_id in available_factories) <= 1, f"RestricaoCrianca_{child_id}"

    # Restrições de distribuição mínima dentro de cada país
    country_distributions = {country_id: lpSum(0) for country_id in countries.keys()}
    for (child_id, factory_id), var in x.items():
        factory_country = factories[factory_id][0]  # País da fábrica
        country_distributions[factory_country] += var

    for country_id, (p_max, p_min) in countries.items():
        problem += country_distributions[country_id] <= p_max, f"RestricaoDistribuicaoMax_pais_{country_id}"
        problem += country_distributions[country_id] >= p_min, f"RestricaoDistribuicaoMin_pais_{country_id}"

    # Resolver o problema
    solver = PULP_CBC_CMD(msg=False)
    problem.solve(solver)

    print(int(problem.objective.value()))

    # Saída do resultado
    if problem.status != LpStatusOptimal:
        print(-1)
    else:
        print(int(problem.objective.value()))

if __name__ == "__main__":
    input_data = sys.stdin.read()
    solve_toy_distribution(input_data)
