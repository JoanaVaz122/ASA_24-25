from pulp import LpMaximize, LpProblem, LpVariable, lpSum, GLPK
import sys

def maximize_toy_distribution(num_factories, num_countries, num_children, factories, countries, requests):
    # Create the linear programming problem
    model = LpProblem(name="Maximize_Toy_Distribution", sense=LpMaximize)

    # Define variables
    # Factory stock variables (non-negative integers with upper bound)
    #factory_vars = {i: LpVariable(name=f"x{i}", lowBound=0, upBound=factories[i][2], cat='Integer') 
    #                for i in range(num_factories)}
    
    # Child satisfaction variables (binary: 0 or 1)
    #child_vars = {k: LpVariable(name=f"Child_{k+1}_Satisfied", lowBound=0, upBound=factories[i][2],cat='Binary') for k in range(num_children)}

    # variables 
    request_vars = {k: LpVariable(name=f"Request_{k+1}_Satisfied", lowBound=0, upBound=factories[k][2], cat='Binary') for k in range(num_factories)}
    
    # Objective function: Maximize the number of satisfied children
    model += lpSum(request_vars[k] for k in range(num_factories))

    # Constraints
    # Factory production constraints: maximum production per factory
    factory_vars = {i: LpVariable(name=f"Factory_{i+1}_Stock", lowBound=0, upBound=factories[i][2], cat='Integer') 
                    for i in range(num_factories)}
    for i in range(num_factories):
        model += factory_vars[i] <= factories[i][2], f"Factory_{i+1}_Max_Stock"



    # Country export constraints: respect export limits and minimum distributions
    country_exports = {j: lpSum(factory_vars[i] for i in range(num_factories) if factories[i][1] == j) 
                       for j in range(1, num_countries + 1)}
    for j in range(1, num_countries + 1):
        model += country_exports[j] <= countries[j-1][1], f"Country_{j}_Export_Limit"
        model += country_exports[j] >= countries[j-1][2], f"Country_{j}_Min_Distribution"


    # Requests: A child can only be satisfied by the factories they requested
    for k in range(num_children):
        child_request = requests[k]
        child_factories = child_request[2:]
        
        # Constraint to ensure that the child is satisfied if the requested factories provide toys
        model += lpSum(factory_vars[f-1] for f in child_factories) >= request_vars[k], f"Child_{k+1}_Satisfaction"
        
    # Each child can only be satisfied by one request
    for k in range(num_children):
        model += request_vars[k] <= 1, f"Child_{k+1}_Max_One_Request"

    # Solve the problem
    model.solve(GLPK(msg=False))

    # Return the maximum number of satisfied children
    if model.status != 1:  # 1 corresponds to LpStatusOptimal
        return -1

    return int(model.objective.value())


def main():
    # Read input
    n, m, t = map(int, input().split())
    
    # Factories: List of tuples (factory_id, country_id, max_stock)
    factories = [list(map(int, input().split())) for _ in range(n)]  # Example: (1 1 1)
    
    # Countries: List of tuples (country_id, max_export, min_distribution)
    countries = [list(map(int, input().split())) for _ in range(m)]  # Example: (1 2 1)
    
    # Requests: List of tuples (child_id, country_id, list of factory_ids requested)
    requests = [list(map(int, input().split())) for _ in range(t)]  # Example: (1 1 2 3)

    # Solve the problem
    result = maximize_toy_distribution(n, m, t, factories, countries, requests)
    print(result)

main()
