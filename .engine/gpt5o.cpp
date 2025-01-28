#ifndef CHATGPT5_H
#define CHATGPT5_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <stdexcept>
#include <regex>
#include <memory>
#include "config.h"  // Assume this file contains configuration settings
#include "logger.h"  // Assume this file handles logging
#include "command.h"  // Assume this file implements the command pattern for task execution
#include "rate_limiter.h"  // Assume this file manages rate limiting

namespace chatbot {

constexpr size_t MAX_INPUT_LENGTH = 1000;
constexpr size_t MAX_MEMORY_ENTRIES = 1000;
constexpr size_t MAX_KEY_LENGTH = 100;
constexpr size_t MAX_VALUE_LENGTH = 1000;

class ChatGPT5 {
private:
    // Database of responses for basic interaction
    std::vector<std::string> responses = {
        "Hello! How can I help you today?",
        "I'm here to assist with any questions you might have.",
        "That's an interesting question. Let me think about it...",
        "I'm not sure about that. Can you ask in another way?"
    };

    // Structure to hold enabled features of the AI model
    struct Feature {
        std::string name;
        bool isEnabled;
        virtual bool apply(const std::string& input) = 0;
        virtual ~Feature() = default;
    };

    // List of features this AI model supports
    std::vector<std::unique_ptr<Feature>> features = {
        std::make_unique<FeatureImpl>("Context Understanding", true),
        std::make_unique<FeatureImpl>("Zero-shot Learning", true),
        std::make_unique<FeatureImpl>("Code Generation", true),
        std::make_unique<FeatureImpl>("Image Understanding", true),
        std::make_unique<FeatureImpl>("Multilingual Support", true),
        std::make_unique<FeatureImpl>("Reasoning", true),
        std::make_unique<FeatureImpl>("Personalization", true),
        std::make_unique<FeatureImpl>("Task Execution", true),
        std::make_unique<FeatureImpl>("Web Interaction", true)
    };

    // Persistent Memory 
    std::map<std::string, std::string> memory;

    // User Profile for personalization
    std::map<std::string, std::vector<std::string>> userProfile;

    void loadConfiguration(const std::string& config_path);
    void initializeFeatures();
    void saveState();
    void cleanupOldestMemory();

public:
    explicit ChatGPT5(const std::string& config_path = "default_config.json") {
        try {
            loadConfiguration(config_path);
            initializeFeatures();
            logger::info("ChatGPT5 initialized successfully.");
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to initialize ChatGPT5: " + std::string(e.what()));
        }
    }

    ~ChatGPT5() {
        try {
            saveState();
        } catch (const std::exception& e) {
            logger::error("Error during cleanup: {}", e.what());
        }
    }

    void initialize() {
        logger::info("ChatGPT5 initialized.");
    }

    std::string processInput(const std::string& input) {
        if (input.empty() || input.length() > MAX_INPUT_LENGTH) {
            throw std::invalid_argument("Invalid input length");
        }

        try {
            std::string sanitized_input = sanitizeInput(input);
            if (!rate_limiter::allowRequest()) {
                return "Please wait before sending another message.";
            }

            std::string response = selectBasicResponse(sanitized_input);
            
            // Use command pattern instead of string matching
            auto command = CommandFactory::createCommand(sanitized_input);
            if (command) {
                response += command->execute();
            }

            for (const auto& feature : features) {
                if (feature->isEnabled) {
                    feature->apply(sanitized_input);
                }
            }

            return response;
        } catch (const std::exception& e) {
            logger::error("Error processing input: {}", e.what());
            return "I encountered an error processing your request.";
        }
    }

private:
    std::string selectBasicResponse(const std::string& input) {
        auto normalized = toLower(input);
        
        static const std::vector<std::pair<std::regex, int>> intentPatterns = {
            {std::regex("\\b(hello|hi)\\b", std::regex::icase), 0},
            {std::regex("\\?"), 2}
        };

        for (const auto& pattern : intentPatterns) {
            if (std::regex_search(normalized, pattern.first)) {
                return responses[pattern.second];
            }
        }
        
        return responses[3];
    }

    void handleMemory(const std::string& input) {
        if (memory.size() >= MAX_MEMORY_ENTRIES) {
            cleanupOldestMemory();
        }
        
        auto command = MemoryCommandParser::parse(input);
        if (!command) {
            throw std::invalid_argument("Invalid memory command");
        }
        
        if (command->type == MemoryCommandType::Remember) {
            if (command->key.length() > MAX_KEY_LENGTH || 
                command->value.length() > MAX_VALUE_LENGTH) {
                throw std::length_error("Memory entry too large");
            }
            memory[command->key] = command->value;
            logger::info("Stored in memory.");
        } else if (command->type == MemoryCommandType::Recall) {
            auto iter = memory.find(command->key);
            if (iter != memory.end()) {
                logger::info("Recalled from memory: {}", iter->second);
            } else {
                logger::warn("No memory found for: {}", command->key);
            }
        }
    }

    void personalizeResponse(const std::string& input) {
        std::string userKey = "current_user";  // Assume we know who's talking
        userProfile[userKey].push_back(input);  // Store conversation history
        logger::info("Response tailored for user's history.");
    }

    std::string executeTask(const std::string& input) {
        if (input.find("book") != std::string::npos) {
            return "\nBooked flight for ";
        } else if (input.find("order") != std::string::npos) {
            return "\nOrdered ";
        }
        return ""; // No task to execute
    }

    std::string applyFeatures(const std::string& input) {
        return "\nAll features applied.";
    }

    std::string sanitizeInput(const std::string& input) {
        // Implement input sanitization here, e.g., remove scripts, sanitize HTML
        return input; // Placeholder
    }

    std::string toLower(const std::string& s) {
        std::string result = s;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return result;
    }
};

// FeatureImpl is a placeholder for actual feature implementations
struct FeatureImpl : public Feature {
    FeatureImpl(std::string n, bool e) : Feature{n, e} {}
    bool apply(const std::string& input) override {
        // Placeholder for feature application logic
        return true;
    }
};

} // namespace chatbot

#endif // CHATGPT5_H
