from pulp import LpMaximize, LpProblem, LpVariable, lpSum, GLPK
import sys

def maximize_toy_distribution(num_factories, num_countries, num_children, factories, countries, requests):
    # Create the linear programming problem
    model = LpProblem(name="Maximize_Toy_Distribution", sense=LpMaximize) 

    # Define variables
    child_vars = {k: LpVariable(name=f"child_{k}", cat="Binary") for k in range(1, num_children + 1)}

    factory_vars = {i: LpVariable(name=f"factory_{i}", lowBound=0, upBound=factories[i-1][2], cat="Integer") for i in range(1, num_factories + 1)}

    # Objective function: Maximize the number of satisfied children
    model += lpSum(child_vars[k] for k in range(1, num_children + 1))

    # Constraints
    # Factory production constraints
    for i in range(1, num_factories + 1):
        model += factory_vars[i] <= factories[i-1][2], f"Factory_{i}_Max_Stock"

    # Country constraints: respect export limits and minimum distributions
    country_exports = {j: lpSum(factory_vars[i] for i, _, stock in factories if factories[i-1][1] == j)
                       for j in range(1, num_countries + 1)}
    for j in range(1, num_countries + 1):
        model += country_exports[j] <= countries[j-1][1], f"Country_{j}_Export_Limit"
        model += country_exports[j] >= countries[j-1][2], f"Country_{j}_Min_Distribution"

    # Requests: a child can only receive toys from factories in their request
    for k in range(1, num_children + 1):
        child_request = requests[k-1]
        child_country = child_request[1]
        child_factories = child_request[2:]
        model += lpSum(factory_vars[f] for f in child_factories) >= child_vars[k], f"Child_{k}_Satisfaction"

    # Solve the problem
    model.solve(GLPK(msg=False))

    # Check if the solution is feasible
    if model.status != 1:  # 1 corresponds to LpStatusOptimal
        return -1

    # Return the maximum number of satisfied children
    return int(model.objective.value())

def main():
    # Read input
    input_data = sys.stdin.read().splitlines()
    n, m, t = map(int, input_data[0].split())
    
    factories = [tuple(map(int, line.split())) for line in input_data[1:n+1]]
    countries = [tuple(map(int, line.split())) for line in input_data[n+1:n+1+m]]
    requests = [tuple(map(int, line.split())) for line in input_data[n+1+m:]]
    
    # Solve the problem
    result = maximize_toy_distribution(n, m, t, factories, countries, requests)
    print(result)

main()
