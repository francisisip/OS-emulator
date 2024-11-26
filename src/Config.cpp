#include "Config.h"
#include <stdexcept>

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
    maxMemory = validateRange("max-overall-mem", std::stoul(configMap["max-overall-mem"]), 2, UINT32_MAX);
    memoryPerFrame = validateRange("mem-per-frame", std::stoul(configMap["mem-per-frame"]), 2, UINT32_MAX);
    minMemoryPerProcess = validateRange("min-mem-per-proc", std::stoul(configMap["min-mem-per-proc"]), 2, UINT32_MAX);

    maxMemoryPerProcess = validateRange("max-mem-per-proc", std::stoul(configMap["max-mem-per-proc"]), 2, UINT32_MAX);
    // Additional Logic to ensure memory is in power of 2
    validatePowerOfTwo(maxMemory);
    validatePowerOfTwo(memoryPerFrame);
    validatePowerOfTwo(minMemoryPerProcess);
    validatePowerOfTwo(maxMemoryPerProcess);
    // Additional logic to ensure `min-ins` <= `max-ins`
    if (minIns > maxIns) {
        throw std::runtime_error("min-ins cannot be greater than max-ins");
    }
    // Additional logic to ensure min-mem-per-proc <= max-mem-per-proc
    if (minMemoryPerProcess > maxMemoryPerProcess) {
        throw std::runtime_error("min-mem-per-proc cannot be greater than max-mem-per-proc");
    }
}

// Helper to validate CPU count
int Config::validateNumCpu(int numCpu) {
    if (numCpu < 1 || numCpu > 128) {
        throw std::out_of_range("num-cpu must be in the range [1, 128]");
    }
    return numCpu;
}
void Config::validatePowerOfTwo(unsigned int memory){
  // memory is power of two
  if((memory & (memory - 1)) != 0){
      throw std::runtime_error("memory value has to be power of 2 format");
  }
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
unsigned int Config::getMaxMemory() const { return maxMemory; }
unsigned int Config::getMemoryPerFrame() const { return memoryPerFrame; }
unsigned int Config::getMinMemoryPerProcess() const { return minMemoryPerProcess; }
unsigned int Config::getMaxMemoryPerProcess() const { return maxMemoryPerProcess; }

