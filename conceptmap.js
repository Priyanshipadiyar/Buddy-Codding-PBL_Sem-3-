const concepts = {
    "Basic Variables": ["Loops"],
    "Loops": ["Arrays"],
    "Arrays": ["Functions"],
    "Functions": []
};

// BFS: concepts ko level-by-level visit karna
function bfs(startConcept) {
    const queue = [startConcept];
    const visited = new Set([startConcept]);
    const order = [];

    while (queue.length > 0) {
        const current = queue.shift();
        order.push(current);

        for (const neighbour of concepts[current] || []) {
            if (!visited.has(neighbour)) {
                visited.add(neighbour);
                queue.push(neighbour);
            }
        }
    }

    return order;
}

// Concept Map ko screen par dikhana
function showConceptMap() {
    const result = bfs("Basic Variables");
    const conceptInfo = document.getElementById("conceptInfo");

    conceptInfo.innerHTML = "";

    result.forEach((concept, index) => {
        const item = document.createElement("p");

        item.textContent =
            (index + 1) + ". " + concept + " ✓";

        item.style.padding = "12px";
        item.style.margin = "8px 0";
        item.style.borderRadius = "8px";
        item.style.background = "#d9f3e5";
        item.style.borderLeft = "5px solid #198754";

        conceptInfo.appendChild(item);
    });

    const orderText = document.createElement("p");
    orderText.textContent =
        "BFS Traversal Order: " + result.join(" → ");

    orderText.style.fontWeight = "bold";
    orderText.style.marginTop = "15px";

    conceptInfo.appendChild(orderText);
}

showConceptMap();