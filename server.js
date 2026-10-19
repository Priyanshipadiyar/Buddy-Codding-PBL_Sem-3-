const express = require('express');
const cors = require('cors');
const { execFile } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());

const CPP_BINARY_PATH = path.join(__dirname, 'backend', 'buddy_coding');
const tempDir = path.join(__dirname, 'backend', 'CodeChecker', 'temp');

if (!fs.existsSync(tempDir)) {
    fs.mkdirSync(tempDir, { recursive: true });
}

app.post('/api/submit', (req, res) => {
    console.log("Received submission request for user:", req.body.userId);
    const { userId, problemId, sourceCode } = req.body;

    if (!userId || !problemId || !sourceCode) {
        return res.status(400).json({ error: "Missing required fields" });
    }

    const sourcePath = path.join(tempDir, `student_${userId}.cpp`);
    fs.writeFileSync(sourcePath, sourceCode);

    // FIX 1: Removed 'submit' so argc == 4 in C++, triggering Headless Mode safely
    execFile(CPP_BINARY_PATH, [userId, problemId, sourcePath], (error, stdout, stderr) => {
        if (error) {
            console.error("Backend execution error:", stderr || error.message);
            return res.status(500).json({ error: "C++ backend execution failed.", details: stderr || error.message });
        }

        // FIX 2: Parse the plain text output from C++ into a structured JSON object
        try {
            const result = {};
            const lines = stdout.split('\n');
            
            lines.forEach(line => {
                const separatorIndex = line.indexOf(': ');
                if (separatorIndex !== -1) {
                    const key = line.substring(0, separatorIndex).trim();
                    const value = line.substring(separatorIndex + 2).trim();
                    result[key] = value;
                }
            });

            // Send the cleanly parsed JSON back to your frontend
            res.json(result);
            
        } catch (e) {
            res.status(500).json({ error: "Failed to parse output from C++ backend", rawOutput: stdout });
        }
    });
});

app.listen(3000, () => {
    console.log('Buddy Coding Bridge Server running on http://localhost:3000');
});