#include "scip/scip.h"
#include "scip/scipdefplugins.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include "reader.h"   // assume readFile returns pair<int, vector<int>>

using namespace std;

int main() {
    SCIP* scip = nullptr;

    // Create SCIP environment
    SCIP_CALL(SCIPcreate(&scip));
    SCIP_CALL(SCIPincludeDefaultPlugins(scip));
    SCIP_CALL(SCIPcreateProbBasic(scip, "identical_machine_scheduling"));

    // Parse the file
    auto results = readFile("data.txt"); // TODO: add command-line arg
    int m = results.first;
    vector<int> p = results.second;
    int n = (int)p.size();

    // Basic input validation
    if (m <= 0) {
        cerr << "Error: number of machines m must be > 0 (got " << m << ")\n";
        SCIP_CALL(SCIPfree(&scip));
        return 1;
    }
    if (n <= 0) {
        cerr << "Error: number of jobs n must be > 0 (got " << n << ")\n";
        SCIP_CALL(SCIPfree(&scip));
        return 1;
    }

    cout << "Read m = " << m << ", n = " << n << " jobs\n";

    // Create variables x[j][i] for job j and machine i
    vector<vector<SCIP_VAR*>> x(n, vector<SCIP_VAR*>(m, nullptr));
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            char vname[64];
            snprintf(vname, sizeof(vname), "x_%d_%d", j, i);
            SCIP_CALL(SCIPcreateVarBasic(scip, &x[j][i], vname,
                                         0.0, 1.0, 0.0, SCIP_VARTYPE_BINARY));
            SCIP_CALL(SCIPaddVar(scip, x[j][i]));
        }
    }

    // Create variable T (makespan)
    SCIP_VAR* T = nullptr;
    SCIP_CALL(SCIPcreateVarBasic(scip, &T, "T", 0.0, SCIPinfinity(scip), 1.0, SCIP_VARTYPE_CONTINUOUS));
    SCIP_CALL(SCIPaddVar(scip, T));

    // Constraints: load on each machine ≤ T
    vector<SCIP_CONS*> loadCons(m, nullptr);
    for (int i = 0; i < m; ++i) {
        char cname[64];
        snprintf(cname, sizeof(cname), "load_%d", i);
        SCIP_CALL(SCIPcreateConsBasicLinear(scip, &loadCons[i], cname,
                                            0, nullptr, nullptr,
                                            -SCIPinfinity(scip), 0.0));
        for (int j = 0; j < n; ++j) {
            SCIP_CALL(SCIPaddCoefLinear(scip, loadCons[i], x[j][i], (SCIP_Real)p[j]));
        }
        SCIP_CALL(SCIPaddCoefLinear(scip, loadCons[i], T, -1.0));
        SCIP_CALL(SCIPaddCons(scip, loadCons[i]));
    }

    // Constraints: each job assigned exactly once
    vector<SCIP_CONS*> assignCons(n, nullptr);
    for (int j = 0; j < n; ++j) {
        char cname[64];
        snprintf(cname, sizeof(cname), "assign_%d", j);
        SCIP_CALL(SCIPcreateConsBasicLinear(scip, &assignCons[j], cname,
                                            0, nullptr, nullptr,
                                            1.0, 1.0));
        for (int i = 0; i < m; ++i) {
            SCIP_CALL(SCIPaddCoefLinear(scip, assignCons[j], x[j][i], 1.0));
        }
        SCIP_CALL(SCIPaddCons(scip, assignCons[j]));
    }

    // Objective: minimize T
    SCIP_CALL(SCIPsetObjsense(scip, SCIP_OBJSENSE_MINIMIZE));

    // Optional: print model size for sanity-check
    cout << "Model constructed: vars = " << n*m + 1 << ", loadCons = " << m
         << ", assignCons = " << n << "\n";

    // Solve
    SCIP_CALL(SCIPsolve(scip));

    // Print solution if any
    SCIP_SOL* sol = SCIPgetBestSol(scip);
    if (sol != nullptr) {
        cout << "Makespan T = " << SCIPgetSolVal(scip, sol, T) << "\n";
    } else {
        cout << "No solution found.\n";
    }

    // Cleanup: release constraints and variables (reverse order of creation is fine)
    for (int j = 0; j < n; ++j) {
        SCIP_CALL(SCIPreleaseCons(scip, &assignCons[j]));
    }
    for (int i = 0; i < m; ++i) {
        SCIP_CALL(SCIPreleaseCons(scip, &loadCons[i]));
    }

    // release variables
    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            SCIP_CALL(SCIPreleaseVar(scip, &x[j][i]));
        }
    }
    SCIP_CALL(SCIPreleaseVar(scip, &T));

    // Free SCIP
    SCIP_CALL(SCIPfree(&scip));
    return 0;
}
