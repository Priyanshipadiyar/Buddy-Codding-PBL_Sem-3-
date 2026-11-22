const express = require('express');
const cors = require('cors');
const { execFile } = require('child_process');
const fs = require('fs');
const path = require('path');

const app = express();
app.use(cors());
app.use(express.json());

const CPP_BINARY_PATH = path.join(__dirname, 'buddy_coding');
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

    const relativeSourcePath = path.join('backend', 'CodeChecker', 'temp', `student_${userId}.cpp`);
    const absoluteSourcePath = path.join(__dirname, relativeSourcePath);
    fs.writeFileSync(absoluteSourcePath, sourceCode);

    execFile(CPP_BINARY_PATH, [userId, problemId, relativeSourcePath], { cwd: __dirname }, (error, stdout, stderr) => {
        if (error) {
            console.error("Backend execution error:", stderr || error.message);
            return res.status(500).json({
                error: "C++ backend execution failed.",
                details: stderr || error.message,
                rawOutput: stdout
            });
        }

        try {
            const result = {};
            let currentKey = null;
            const lines = stdout.split('\n');

            lines.forEach(line => {
                const match = line.match(/^([A-Z0-9_]+):\s*(.*)$/);
                if (match) {
                    currentKey = match[1];
                    result[currentKey] = match[2].trim();
                } else if (currentKey && line.trim().length > 0) {
                    result[currentKey] += '\n' + line;
                }
            });

            // Attach failing test case Input, Expected Output, and Actual Output
            if (result.STATUS === 'FAIL' && result.FAILED_TEST) {
                const tcName = result.FAILED_TEST.trim(); // e.g., "T1"
                const inFile = path.join(__dirname, 'test_cases', problemId, `${tcName}_in.txt`);
                const outFile = path.join(__dirname, 'test_cases', problemId, `${tcName}_out.txt`);
                const actualFile = path.join(__dirname, 'student_output.txt');

                const details = [];
                if (fs.existsSync(inFile)) {
                    details.push(`Input:\n${fs.readFileSync(inFile, 'utf8').trim()}`);
                }
                if (fs.existsSync(outFile)) {
                    details.push(`Expected Output:\n${fs.readFileSync(outFile, 'utf8').trim()}`);
                }
                if (fs.existsSync(actualFile)) {
                    const actual = fs.readFileSync(actualFile, 'utf8').trim();
                    details.push(`Your Output:\n${actual.length > 0 ? actual : '(empty)'}`);
                }

                if (details.length > 0) {
                    result.FAILED_TEST_DETAILS = details.join('\n\n');
                }
            }

            if (Object.keys(result).length === 0 && stdout.trim().length > 0) {
                result.Output = stdout.trim();
            }

            res.json(result);

        } catch (e) {
            res.status(500).json({ error: "Failed to parse output from C++ backend", rawOutput: stdout });
        }
    });
});

app.post('/api/next-hint', (req, res) => {
    const { userId, problemId } = req.body;
    if (!userId || !problemId) {
        return res.status(400).json({ error: "Missing userId or problemId" });
    }

    execFile(CPP_BINARY_PATH, ['--next-hint', userId, problemId], { cwd: __dirname }, (error, stdout, stderr) => {
        if (error) {
            return res.status(500).json({ error: stderr || error.message });
        }

        const result = {};
        stdout.split('\n').forEach(line => {
            const match = line.match(/^([A-Z0-9_]+):\s*(.*)$/);
            if (match) result[match[1]] = match[2].trim();
        });
        res.json(result);
    });
});

app.listen(3000, '0.0.0.0', () => {
    console.log('Buddy Coding Bridge Server running on http://127.0.0.1:3000');
});
