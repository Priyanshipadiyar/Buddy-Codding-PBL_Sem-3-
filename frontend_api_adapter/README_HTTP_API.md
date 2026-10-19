# Frontend API Adapter Boundary

To connect Buddy Coding to a Web Frontend, you should expose the `Controller` over HTTP. 
Since no web dependency is currently natively configured, we recommend using `cpp-httplib` (a single header-only C++ library) as a clean boundary.

## Example Integration (API Server)

```cpp
#include "httplib.h"
#include "../backend/Controller.hpp"
#include "../logic_engine/json.hpp" // Utilizing your existing nlohmann json

void startServer(Controller* controller) {
    httplib::Server svr;

    // GET /questions -> list available questions
    svr.Get("/questions", [&](const httplib::Request&, httplib::Response& res) {
        // Build JSON array using db_->listProblems()
    });

    // POST /submit -> submit code and return true evaluation
    svr.Post("/submit", [&](const httplib::Request& req, httplib::Response& res) {
        // 1. Extract problemId, sourceCode from req.body JSON
        // 2. Save sourceCode to a temporary file
        // 3. auto result = controller->submitCode(problemId, tempFilePath, "web_session");
        // 4. Serialize result struct to JSON and send
    });

    // POST /hint -> progressive hint requested
    svr.Post("/hint", [&](const httplib::Request& req, httplib::Response& res) {
        // 1. Extract problemId from req.body
        // 2. string hint = controller->getNextHint(problemId, "web_session");
        // 3. Return hint in JSON
    });

    std::cout << "Backend API listening on port 8080..." << std::endl;
    svr.listen("0.0.0.0", 8080);
}
```
