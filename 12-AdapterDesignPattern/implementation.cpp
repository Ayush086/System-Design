/* Adapter Design Pattern                                                                                   Date: 28/05/2026 */
#include <iostream>
#include <string>
using namespace std;

// target interface (client expected)
class IReport {
public:
    virtual string getJSONData(const string& data) = 0;
    virtual ~IReport() {}
};

// adaptee : provides XML data
class XMLDataProvider {
public:
    // expected format: "name:id"
    string getXMLData(const string& data) {
        size_t sep = data.find(':');
        string name = data.substr(0, sep);
        string id = data.substr(sep+1);

        // xml representation
        return "<user>"
               "<name>" + name + "</name>"
               "<id>" + id + "</id>"
               "</user>";
    }
};

// adapter: converts XML -> JSON
class XMLDataProviderAdapter : public IReport{
private:
    XMLDataProvider* xmlProvider;
public:
    XMLDataProviderAdapter(XMLDataProvider* provider){
        this->xmlProvider = provider;
    }

    string getJSONData(const string& data) override {
        // get XML from adaptee
        string xml = xmlProvider->getXMLData(data);

        // parse out the fields
        size_t startName = xml.find("<name>") + 6;
        size_t endName = xml.find("</name>");
        string name = xml.substr(startName, endName - startName);

        size_t startId = xml.find("<id>") + 4;
        size_t endId = xml.find("</id>");
        string id = xml.substr(startId, endId - startId);

        return "{\"name\":\"" + name + "\", \"id\":" + id + "}";
    }
};


// client interacts with IReports only
class Client {
public:
    void getReport(IReport* report, string rawData){
        cout << "Processed JSON: " << report->getJSONData(rawData) << endl;
    }
};


int main() {
    // create adaptee
    XMLDataProvider* xmlProvider = new XMLDataProvider();

    // adapter
    IReport* adapter = new XMLDataProviderAdapter(xmlProvider);

    // provide raw data
    string rawData = "yash: 22";

    // extract json
    Client* client = new Client();
    client->getReport(adapter, rawData);

    delete adapter;
    delete xmlProvider;
    return 0;
}