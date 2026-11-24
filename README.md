# Nexus AI — C++ Smart Chatbot

Nexus AI is a lightweight, menu-driven conversational chatbot built in **C++17**. It demonstrates object-oriented programming, string processing, file handling, command parsing and a simple editable knowledge base.

> **Project type:** Educational rule-based chatbot. It does not generate answers with a large language model. Unknown questions receive a helpful prompt, and users can teach the bot new question-answer pairs.

## Features

- Clean terminal interface with colored headings (where ANSI colors are supported)
- Greetings and responses to common topics
- Built-in commands for help, date, time, history, clear and exit
- Teach the chatbot new answers with `/teach`
- Persistent knowledge stored in `data/knowledge.txt`
- Chat transcript saved to `data/chat_history.txt`
- C++17 and standard library only — no third-party dependencies

## Project structure

```text
Nexus-AI/
├── main.cpp
├── CMakeLists.txt
├── README.md
├── LICENSE
├── .gitignore
├── data/
│   └── knowledge.txt
└── docs/
    └── PROJECT_DOCUMENTATION.md
```

## Requirements

- A C++17 compiler (GCC 8+, Clang 7+, or a recent Visual Studio)
- CMake 3.16+ (optional, but recommended)

## Build and run

### Option A: Compile directly

**Linux / macOS**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o nexus-ai
./nexus-ai
```

**Windows (MinGW)**
```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o nexus-ai.exe
nexus-ai.exe
```

### Option B: Use CMake

```bash
cmake -S . -B build
cmake --build build
```

Run the executable produced inside the `build` folder (the exact path depends on your operating system and generator).

**Important:** Run the program from the project root folder so it can find `data/knowledge.txt` and save the chat history in `data/`.

## Commands

| Command | Purpose |
|---|---|
| `/help` | Show available commands |
| `/time` | Show local time |
| `/date` | Show current date |
| `/history` | Count saved messages |
| `/teach question \| answer` | Add a question and answer |
| `/clear` | Clear the terminal |
| `/exit` | Exit the chatbot |

Example:
```text
You > /teach capital of japan | Tokyo
Nexus > Knowledge saved! Ask me "capital of japan" next time.

You > capital of japan
Nexus > Tokyo
```

The knowledge file uses one entry per line in this format:

```text
question phrase|answer text
```

Avoid using the `|` character inside a question or answer because it separates the two fields.

## GitHub upload

1. Sign in to [GitHub](https://github.com/).
2. Select **New repository**.
3. Name it `nexus-ai-cpp`.
4. Add a short description: `A C++17 rule-based chatbot with a persistent knowledge base.`
5. Choose Public or Private, then create the repository.
6. Open a terminal inside this project folder and run:

```bash
git init
git add .
git commit -m "Initial commit: Nexus AI C++ chatbot"
git branch -M main
git remote add origin https://github.com/YOUR-USERNAME/nexus-ai-cpp.git
git push -u origin main
```

Replace `YOUR-USERNAME` with your GitHub username. Do not upload passwords, API keys, personal data or private chat logs.

## Future improvements

- Connect a trusted AI service through a secure backend
- Add a graphical interface using Qt
- Add unit tests and continuous integration
- Improve fuzzy matching and multilingual support

## License

This project is released under the MIT License. See [LICENSE](LICENSE).
