import sys
import pulp

def main():
    input = sys.stdin
    n, m, t = map(int, input.readline().strip().split())  
    
    factories = [list(map(int, input.readline().strip().split())) for _ in range(n)]
    countries = [list(map(int, input.readline().strip().split())) for _ in range(m)]
    requests = [list(map(int, input.readline().strip().split())) for _ in range(t)]
    
    # Create the LP problem  
    prob = pulp.LpProblem("Maximize_Children_Satisfaction", pulp.LpMaximize)
    
    # Create variables
    x = {}
    for k in range(t):
        for i in range(2, len(requests[k])):
            x[(k, requests[k][i] - 1)] = pulp.LpVariable(f"x_{k}_{requests[k][i] - 1}", cat='Binary')
    
    # Objective function
    prob += pulp.lpSum(x.values())
    
    # Constraint 1: Each factory cant exceed its capacity
    for i in range(n):
        prob += pulp.lpSum(x[(k, i)] for k in range(t) if (k, i) in x) <= factories[i][2], f"Factory_{i}_Capacity"
    
    
    for j in range(m):
        # Constraint 2: Each country cant exceed its export limit
        prob += pulp.lpSum(x[(k, i)] for k in range(t) if requests[k][1] != j + 1 for i in range(n) if (k, i) in x and factories[i][1] == j + 1) <= countries[j][1], f"Country_{j}_Max_Export"
        
        # Constraint 3: Each country must satisfy its minimum requests
        prob += pulp.lpSum(x[(k, i)] for k in range(t) if requests[k][1] == j + 1 for i in range(n) if (k, i) in x) >= countries[j][2], f"Country_{j}_Min_Requests_Satisfied"
    
    # Constraint 4: Each request must be satisfied by a single factory
    for k in range(t):
        prob += pulp.lpSum(x[(k, i)] for i in range(n) if (k, i) in x) <= 1, f"Request_{k}_Single_Factory"
    
    # Solve the problem
    prob.solve(pulp.PULP_CBC_CMD(msg=False))
    
    # Output the result
    if pulp.LpStatus[prob.status] == 'Optimal':
        print(int(pulp.value(prob.objective)))
    else:
        print(-1)
        
if __name__ == "__main__":
    main()
