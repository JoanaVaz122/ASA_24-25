from pulp import LpMaximize, LpProblem, LpVariable, lpSum, GLPK

def maximize_profit(num_toys, num_packages, max_toys, toy_data, package_data):
    # Create a linear programming problem
    model = LpProblem(name="Maximize_Profit", sense=LpMaximize)

    # Define variables
    toy_vars = {i: LpVariable(name=f"x{i}", lowBound=0, upBound=toy_data[i][1], cat="Integer") for i in range(num_toys)}
    package_vars = {i: LpVariable(name=f"p{i}", lowBound=0, upBound=min(toy_data[j-1][1] for j in package_data[i][:3]), cat="Integer") for i in range(num_packages)}

    # Objective function
    model += lpSum(toy_data[i][0] * toy_vars[i] for i in range(num_toys)) + \
             lpSum(package_data[i][3] * package_vars[i] for i in range(num_packages))

    # Constraints
    model += lpSum(toy_vars[i] for i in range(num_toys)) + lpSum(package_vars[i] * 3 for i in range(num_packages)) <= max_toys

    toy_packages_dict = {i: [] for i in range(num_toys)}

    for j in range(num_packages):
        for i in package_data[j][:3]:
            toy_packages_dict[i - 1].append(j)

    for i in range(num_toys):
        model += lpSum(package_vars[j] for j in toy_packages_dict[i]) + toy_vars[i] <= toy_data[i][1]

    # Solve the problem with GLPK solver
    model.solve(GLPK(msg=False))

    return int(model.objective.value())

def main():
    t, p, max_toys = map(int, input().split())

    toys = {i: tuple(map(int, input().split())) for i in range(t)}
    packages = {i: tuple(map(int, input().split())) for i in range(p)}

    # Call the solver only once
    result = maximize_profit(t, p, max_toys, toys, packages)
    print(result)

main()