import pickle

class PMLLMemory:
    """
    A simple persistent memory class that stores interaction history.
    """
    def __init__(self):
        self.history = []
    
    def save_entry(self, input_text, prediction, feedback=None):
        """Save an interaction entry to the history."""
        entry = {"text": input_text, "prediction": prediction, "feedback": feedback}
        self.history.append(entry)
    
    def get_history(self):
        """Return the entire interaction history."""
        return self.history

def save_memory(memory, filename="memory.pkl"):
    """
    Serialize the memory history to a file.
    """
    with open(filename, "wb") as f:
        pickle.dump(memory.get_history(), f)
    print(f"Memory saved to {filename}.")

def load_memory(memory, filename="memory.pkl"):
    """
    Load the memory history from a file.
    """
    with open(filename, "rb") as f:
        memory.history = pickle.load(f)
    print(f"Memory loaded from {filename}.")

def main():
    # Create an instance of PMLLMemory and simulate storing some interactions.
    memory = PMLLMemory()
    memory.save_entry("Hello", "Hi there!", "Positive feedback")
    memory.save_entry("How are you?", "I'm fine, thanks.", "Neutral response")
    
    print("Memory before saving:")
    print(memory.get_history())
    
    # Save the memory to disk.
    save_memory(memory, "memory.pkl")
    
    # Create a new memory instance and load the persisted data.
    new_memory = PMLLMemory()
    load_memory(new_memory, "memory.pkl")
    
    print("Memory after loading:")
    print(new_memory.get_history())

if __name__ == "__main__":
    main()
