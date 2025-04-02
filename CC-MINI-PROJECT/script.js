function analyzeFunctions(code) {
    // Improved regex patterns for more accurate detection
    const functionTypes = {
        functionDeclaration: /function\s+([a-zA-Z_$][\w$]*)\s*\([^)]*\)/g,
        functionExpression: /(?:const|let|var)\s+([a-zA-Z_$][\w$]*)\s*=\s*function\s*\([^)]*\)/g,
        arrowFunction: /(?:(?:const|let|var)\s+([a-zA-Z_$][\w$]*)\s*=\s*|(?:([a-zA-Z_$][\w$]*)\s*=\s*))(?:\([^)]*\)|\w+)\s*=>/g,
        iife: /\(\s*function\s*\([^)]*\)\s*\{[\s\S]*?\}\s*\)\s*\(\s*\)/g,
        methodDefinition: /([a-zA-Z_$][\w$]*)\s*\([^)]*\)\s*\{/g,
        shorthandMethod: /([a-zA-Z_$][\w$]*)\s*\([^)]*\)/g
    };

    try {
        // Basic syntax validation
        new Function(code);

        let results = {};
        let maxCount = 0;
        let functionDetails = [];

        // Extract code comments to prevent false positives
        const commentsReplaced = code.replace(/\/\/.*$/gm, '').replace(/\/\*[\s\S]*?\*\//g, '');

        for (let type in functionTypes) {
            // Skip method definition for global code   
            if (type === 'methodDefinition' && !code.includes('class ')) {
                continue;
            }

            // Skip shorthand methods unless in object literal context
            if (type === 'shorthandMethod' && !(/\{\s*[\w$]+\s*\([^)]*\)/g.test(code))) {
                continue;
            }

            const matches = Array.from(commentsReplaced.matchAll(functionTypes[type]) || []);
            results[type] = matches.length;
            maxCount = Math.max(maxCount, matches.length);

            // Store actual function matches for detailed view
            matches.forEach(match => {
                const functionName = match[1] || 'Anonymous';
                const fullMatch = match[0];

                // Extract function body (limited capability without proper parsing)
                let body = '';
                if (fullMatch) {
                    const startIndex = code.indexOf(fullMatch);
                    if (startIndex >= 0) {
                        // Simple extraction attempt - this is not perfect but provides some context
                        body = extractFunctionBody(code, startIndex, type);
                    }
                }

                functionDetails.push({
                    type,
                    name: functionName,
                    code: fullMatch.trim(),
                    body: body.trim(),
                    lineNumber: getLineNumber(code, fullMatch)
                });
            });
        }

        return {
            results,
            maxCount,
            functionDetails,
            error: null
        };
    } catch (error) {
        return {
            results: {},
            maxCount: 0,
            functionDetails: [],
            error: {
                message: error.message,
                line: error.lineNumber || 'Unknown'
            }
        };
    }
}

function extractFunctionBody(code, startIndex, type) {
    // This is a simple extraction - a real parser would do better
    if (type === 'arrowFunction') {
        // Find the arrow
        const arrowIndex = code.indexOf('=>', startIndex);
        if (arrowIndex > 0) {
            // Check if it's a block body or expression body
            const nextChar = code.substring(arrowIndex + 2).trim()[0];
            if (nextChar === '{') {
                // It's a block body
                return extractBalancedBraces(code, code.indexOf('{', arrowIndex));
            } else {
                // It's an expression body
                const endOfLine = code.indexOf('\n', arrowIndex);
                return endOfLine > 0 ?
                    code.substring(arrowIndex + 2, endOfLine).trim() :
                    code.substring(arrowIndex + 2).trim();
            }
        }
    } else {
        // For traditional functions, find opening brace after params
        const openBraceIndex = code.indexOf('{', startIndex);
        if (openBraceIndex > 0) {
            return extractBalancedBraces(code, openBraceIndex);
        }
    }
    return '';
}

function extractBalancedBraces(code, openBraceIndex) {
    if (openBraceIndex < 0) return '';

    let braceCount = 1;
    let index = openBraceIndex + 1;

    while (braceCount > 0 && index < code.length) {
        if (code[index] === '{') braceCount++;
        else if (code[index] === '}') braceCount--;
        index++;
    }

    return code.substring(openBraceIndex, index);
}

function getLineNumber(code, match) {
    const index = code.indexOf(match);
    if (index === -1) return 'Unknown';

    const codeUpToMatch = code.substring(0, index);
    return (codeUpToMatch.match(/\n/g) || []).length + 1;
}

function visualizeResults(analysisData) {
    const visualizationDiv = document.getElementById('visualization');
    visualizationDiv.innerHTML = '';

    if (analysisData.error) {
        showError(analysisData.error);
        return;
    }

    // Create tabs for different views
    const tabsContainer = createTabs();
    visualizationDiv.appendChild(tabsContainer);

    // Create content container
    const contentContainer = document.createElement('div');
    contentContainer.id = 'content-container';
    visualizationDiv.appendChild(contentContainer);

    // Initial view
    showChartView(analysisData);
}

function createTabs() {
    const tabsContainer = document.createElement('div');
    tabsContainer.className = 'tabs';

    const tabs = [
        { id: 'chart', text: 'Chart View' },
        { id: 'details', text: 'Detailed View' }
    ];

    tabs.forEach(tab => {
        const button = document.createElement('button');
        button.textContent = tab.text;
        button.className = 'tab-button';
        if (tab.id === 'chart') button.classList.add('active');
        button.onclick = (event) => switchView(tab.id, event);
        tabsContainer.appendChild(button);
    });

    return tabsContainer;
}

function showError(error) {
    const container = document.getElementById('visualization');
    container.innerHTML = `
        <div class="error-container">
            <h3>⚠️ Syntax Error</h3>
            <div class="error-details">
                <p>${error.message}</p>
                ${error.line !== 'Unknown' ? `<p>Line: ${error.line}</p>` : ''}
            </div>
            <div class="error-help">
                <p>Please check your code for:</p>
                <ul>
                    <li>Missing brackets or parentheses</li>
                    <li>Unclosed string literals</li>
                    <li>Invalid syntax</li>
                </ul>
            </div>
        </div>
    `;
}

function showChartView(analysisData) {
    const container = document.getElementById('content-container');
    container.innerHTML = '';

    const { results, maxCount } = analysisData;
    const maxWidth = 90; // Percentage for the widest bar

    // Calculate total functions
    const totalFunctions = Object.values(results).reduce((sum, count) => sum + count, 0);

    // Create summary
    const summary = document.createElement('div');
    summary.className = 'summary-container';
    summary.innerHTML = `
        <h3>Summary</h3>
        <p>Total Functions: ${totalFunctions}</p>
        <p>Function Types Found: ${Object.keys(results).filter(t => results[t] > 0).length}</p>
    `;
    container.appendChild(summary);

    const chartContainer = document.createElement('div');
    chartContainer.className = 'chart-container';

    // Sort function types by count (descending)
    const sortedTypes = Object.keys(results).sort((a, b) => results[b] - results[a]);

    for (let type of sortedTypes) {
        const count = results[type];
        if (count === 0) continue; // Skip empty types

        const percentage = maxCount > 0 ? (count / maxCount) * maxWidth : 0;

        const functionBar = document.createElement('div');
        functionBar.className = 'function-bar';

        const barContainer = document.createElement('div');
        barContainer.className = 'bar-container';

        const label = document.createElement('div');
        label.className = 'bar-label';
        label.textContent = `${formatTypeName(type)}`;

        const countLabel = document.createElement('div');
        countLabel.className = 'count-label';
        countLabel.textContent = count;

        const bar = document.createElement('div');
        bar.className = 'bar';
        bar.style.width = `${percentage}%`;
        bar.style.backgroundColor = getColorForType(type);

        barContainer.appendChild(bar);
        functionBar.appendChild(label);
        functionBar.appendChild(barContainer);
        functionBar.appendChild(countLabel);
        chartContainer.appendChild(functionBar);
    }

    container.appendChild(chartContainer);
}

function showDetailedView(analysisData) {
    const container = document.getElementById('content-container');
    container.innerHTML = '';

    const detailsContainer = document.createElement('div');
    detailsContainer.className = 'details-container';

    // Group by type
    const groupedDetails = {};
    analysisData.functionDetails.forEach(detail => {
        if (!groupedDetails[detail.type]) {
            groupedDetails[detail.type] = [];
        }
        groupedDetails[detail.type].push(detail);
    });

    // Create accordion sections for each type
    for (let type in groupedDetails) {
        const typeSection = document.createElement('div');
        typeSection.className = 'type-section';

        const typeHeader = document.createElement('div');
        typeHeader.className = 'type-header';
        typeHeader.innerHTML = `
            <div class="type-name">${formatTypeName(type)} (${groupedDetails[type].length})</div>
            <div class="type-toggle">▼</div>
        `;
        typeHeader.onclick = () => toggleTypeSection(typeSection);

        const typeContent = document.createElement('div');
        typeContent.className = 'type-content';

        groupedDetails[type].forEach(detail => {
            const functionCard = document.createElement('div');
            functionCard.className = 'function-card';

            // Extract parameters from the function code
            const paramMatch = detail.code.match(/\(([^)]*)\)/);
            const parameters = paramMatch ? paramMatch[1].trim() : '';
            
            // Create syntax-highlighted code display
            const codeDisplay = document.createElement('pre');
            codeDisplay.className = 'function-code';
            const codeElement = document.createElement('code');
            codeElement.textContent = detail.code;
            codeDisplay.appendChild(codeElement);

            // Create function info section with all details
            const functionInfo = document.createElement('div');
            functionInfo.className = 'function-info';
            functionInfo.innerHTML = `
                <div class="function-name">${detail.name || 'Anonymous'}</div>
                <div class="function-meta">
                    <span class="function-type">Type: ${formatTypeName(detail.type)}</span>
                    <span class="function-line">Line: ${detail.lineNumber}</span>
                </div>
                <div class="function-params">Parameters: <code>${parameters || 'none'}</code></div>
            `;

            functionCard.appendChild(functionInfo);
            functionCard.appendChild(codeDisplay);
            typeContent.appendChild(functionCard);
        });

        typeSection.appendChild(typeHeader);
        typeSection.appendChild(typeContent);
        detailsContainer.appendChild(typeSection);
    }

    container.appendChild(detailsContainer);
}

function toggleTypeSection(section) {
    const content = section.querySelector('.type-content');
    const toggle = section.querySelector('.type-toggle');

    if (content.style.display === 'none' || !content.style.display) {
        content.style.display = 'block';
        toggle.textContent = '▼';
    } else {
        content.style.display = 'none';
        toggle.textContent = '►';
    }
}

function switchView(viewType, event) {
    const analysisData = analyzeFunctions(document.getElementById('code-editor').value);

    // Update active tab
    document.querySelectorAll('.tab-button').forEach(button => {
        button.classList.remove('active');
    });
    event.target.classList.add('active');

    if (viewType === 'chart') {
        showChartView(analysisData);
    } else {
        showDetailedView(analysisData);
    }
}

function formatTypeName(type) {
    return type
        .replace(/([A-Z])/g, ' $1')
        .split(/(?=[A-Z])/)
        .join(' ')
        .replace(/^\w/, c => c.toUpperCase());
}

function getColorForType(type) {
    const colors = {
        functionDeclaration: '#4285F4', // Google Blue
        functionExpression: '#EA4335',  // Google Red
        arrowFunction: '#FBBC05',       // Google Yellow
        iife: '#34A853',                // Google Green
        methodDefinition: '#8E24AA',    // Purple
        shorthandMethod: '#FB8C00'      // Orange
    };

    return colors[type] || '#2196F3';
}

function analyzeAndVisualize() {
    const codeEditor = document.getElementById('code-editor');
    const code = codeEditor.value;
    const analysisData = analyzeFunctions(code);
    visualizeResults(analysisData);
}

// Set example code
function setExampleCode() {
    document.getElementById('code-editor').value = `// Example with different function types


`;

    analyzeAndVisualize();
}

// Initial analysis when the page loads
window.onload = function () {
    setExampleCode();
};
