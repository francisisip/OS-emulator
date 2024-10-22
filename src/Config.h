#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>

class Config {
public:
    // Constructor that loads configuration from file
    Config(const std::string& filename);

    // Getters for parameters
    int getNumCpu() const;
    std::string getScheduler() const;
    unsigned int getQuantumCycles() const;
    unsigned int getBatchProcessFreq() const;
    unsigned int getMinIns() const;
    unsigned int getMaxIns() const;
    unsigned int getDelaysPerExec() const;

private:
    // Configuration parameters
    int numCpu;
    std::string scheduler;
    unsigned int quantumCycles;
    unsigned int batchProcessFreq;
    unsigned int minIns;
    unsigned int maxIns;
    unsigned int delaysPerExec;
    unsigned int validateRange(const std::string& paramName, unsigned int value, unsigned int min, unsigned int max);
    std::string validateScheduler(const std::string& scheduler);
    int validateNumCpu(int numCpu);
    void loadConfig(const std::string& filename);
};

#endif // CONFIG_H
