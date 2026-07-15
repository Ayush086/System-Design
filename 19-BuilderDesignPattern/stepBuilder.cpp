/* Step Builder Pattern                                                                                                         Date: 15/07/2026 */
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
using namespace std;


class HTTPRequest {
private:
    string url;
    string method;
    map<string, string> headers;
    map<string, string> queryParams;
    string body;
    int timeout;

    // private constructure (only be accessed by builder)
    HTTPRequest() {}

public:
    friend class HTTPRequestStepBuilder;

    void execute() {
        cout << "Executing " << method << " request to " << url << endl;

        if(!queryParams.empty()) {
            cout << "Query Parameters: " << endl;
            for(const auto& param : queryParams) {
                cout << " " << param.first << "=" << param.second << endl;
            }
        }

        cout << "Headers: " << endl;
        for(const auto& header : headers) {
            cout << " " << header.first << ": " << header.second << endl;
        }

        if(!body.empty()) {
            cout << "Body: " << body << endl;
        }
        cout << "Timeout: " << timeout << " seconds" << endl;
        cout << "Request executed successfully!" << endl;
    }
};


// forward declarations
class MethodStep;
class HeaderStep;
class OptionalStep;

//abstract classes
class URLStep {
public:
    virtual MethodStep& withURL(const string& u) = 0;
};

class MethodStep {
public:
    virtual HeaderStep& withMethod(const string& u) = 0;
};

class HeaderStep {
public:
    virtual OptionalStep& withHeader(string key, string value) = 0;
};

class OptionalStep {
public:
    virtual ~OptionalStep() {}
    virtual OptionalStep& withBody(const string& body) = 0;
    virtual OptionalStep& withTimeout(int timeout) = 0;
    virtual HTTPRequest build() = 0;
};

// concret step builder
class HTTPRequestStepBuilder : 
    public URLStep,
    public MethodStep,
    public HeaderStep,
    public OptionalStep {
        private:
            HTTPRequest req;
        
        public:
            MethodStep& withURL(const string& u) override {
                req.url = u;
                return *this;
            }

            HeaderStep& withMethod(const string& method) override {
                req.method = method;
                return *this;
            }

            OptionalStep& withHeader(string key, string value) override {
                req.headers[key] = value;
                return *this;
            }

            OptionalStep& withBody(const string& body) override {
                req.body = body;
                return *this;
            }

             OptionalStep& withTimeout(int timeout) override {
                req.timeout = timeout;
                return *this;
            }
            
            HTTPRequest build() override {
                if (req.url.empty()) {
                    throw runtime_error("URL cannot be empty");
                }
                return req;
            }

            // to start building
            static URLStep& getBuilder() {
                return *(new HTTPRequestStepBuilder());
            }
};

int main() {
    HTTPRequest stepRequest = HTTPRequestStepBuilder::getBuilder()
    .withURL("https://api.example.com/products")
    .withMethod("POST")
    .withHeader("Content-Type", "application/json")
    .withBody("{\"product\": \"Laptop\", \"price\": 49999}")
    .withTimeout(45)
    .build();

    stepRequest.execute();
}