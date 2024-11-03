#include "Config.h"

Config* Config::instance = nullptr;
Config::Config() {
    loadConfig("config.txt");
}

Config* Config::getInstance(){
    return instance;
}

void Config::initialize() {
    instance = new Config();
}

// Load the configuration from the file
void Config::loadConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open configuration file");
    }

    std::string line;
    std::map<std::string, std::string> configMap;

    // Read the file line by line
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (!(iss >> key >> value)) {
            throw std::runtime_error("Invalid config format");
        }
        configMap[key] = value;
    }

    // Assign and validate values
    numCpu = validateNumCpu(std::stoi(configMap["num-cpu"]));
    scheduler = validateScheduler(configMap["scheduler"]);
    quantumCycles = validateRange("quantum-cycles", std::stoul(configMap["quantum-cycles"]), 1, UINT32_MAX);
    batchProcessFreq = validateRange("batch-process-freq", std::stoul(configMap["batch-process-freq"]), 1, UINT32_MAX);
    minIns = validateRange("min-ins", std::stoul(configMap["min-ins"]), 1, UINT32_MAX);
    maxIns = validateRange("max-ins", std::stoul(configMap["max-ins"]), 1, UINT32_MAX);
    delaysPerExec = validateRange("delay-per-exec", std::stoul(configMap["delay-per-exec"]), 0, UINT32_MAX);

    // Additional logic to ensure `min-ins` <= `max-ins`
    if (minIns > maxIns) {
        throw std::runtime_error("min-ins cannot be greater than max-ins");
    }
}

// Helper to validate CPU count
int Config::validateNumCpu(int numCpu) {
    if (numCpu < 1 || numCpu > 128) {
        throw std::out_of_range("num-cpu must be in the range [1, 128]");
    }
    return numCpu;
}

// Helper to validate scheduler type
std::string Config::validateScheduler(const std::string& scheduler) {
    if (scheduler != "\"fcfs\"" && scheduler != "\"rr\"") {
        throw std::invalid_argument("scheduler must be either 'fcfs' or 'rr'");
    }
    return scheduler;
}

// Generic helper to validate an unsigned integer within a given range
unsigned int Config::validateRange(const std::string& paramName, unsigned int value, unsigned int min, unsigned int max) {
    if (value < min || value > max) {
        throw std::out_of_range(paramName + " must be in the range [" + std::to_string(min) + ", " + std::to_string(max) + "]");
    }
    return value;
}

int Config::getNumCpu() const { return numCpu; }
std::string Config::getScheduler() const { return scheduler; }
unsigned int Config::getQuantumCycles() const { return quantumCycles; }
unsigned int Config::getBatchProcessFreq() const { return batchProcessFreq; }
unsigned int Config::getMinIns() const { return minIns; }
unsigned int Config::getMaxIns() const { return maxIns; }
unsigned int Config::getDelaysPerExec() const { return delaysPerExec; }
