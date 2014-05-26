#pragma once

// Aho-Corasick's string matching algorithm.
// Return a vector of matching positions corresponding to the pattern index.
vector<vi> find(const vector<string> &patterns, string text) {
    // All characters in either patterns or text.
    set<int> characters;

    // Need to calculate for allocation.
    int num_states = 1; // Number of states in DFA.
    for (auto p : patterns) {
        for (int c : p) characters.insert(c);
        num_states += p.size();
    }
    for (int c : text) characters.insert(c);

    // Delta function for our DFA. -1 indicates error.
    vector<vi> delta(num_states, vi(256, -1));

    // fall[s] which state to go to when we can't continue down in trie.
    vi fall(num_states, -1);

    // found[s] indicates all patterns matching at state s.
    vector<set<int>> found(num_states);

    // Build DFA. Start at state 0 and allocate new as needed.
    int last_state = 0;
    for (int i = 0; i < (int)patterns.size(); ++i) {
        int current_state = 0;
        for (char c : patterns[i]) {
            // If we have a new state.
            if (delta[current_state][c] == -1) {
                delta[current_state][c] = ++last_state;
            }
            current_state = delta[current_state][c];
        }
        // At current_state, there is a match of pattern i.
        found[current_state].insert(i);
    }

    // Start state outgoing edge for all characters.
    // Unmatched: goto itself.
    for (int c : characters) {
        if (delta[0][c] == -1)
            delta[0][c] = 0;
    }

    // Build fall table.
    queue<int> q;
    // Initialize with all invalid characters from start.
    for (int c : characters) {
        int next = delta[0][c];
        if (next != -1 && next != 0) {
            fall[next] = 0; // All adjacent to start should just go back.
            q.push(next);
        }
    }
    // BFS update fall edges.
    while (!q.empty()) {
        int state = q.front(); q.pop();
        for (int c : characters) {
            int next_state = delta[state][c];

            // If we cannot go further down, we need a fall edge.
            if (next_state != -1) {
                // Locate nearest valid node.
                int n = fall[state];
                while (delta[n][c] == -1) n = fall[n];
                n = delta[n][c];

                // Insert edge there.
                fall[next_state] = n;

                // If patterns are combined, we can move between.
                // And if we have found at fall edge, we have thus found
                // the same here.
                for (int p : found[n]) {
                    found[next_state].insert(p);
                }

                // Continue bfs.
                q.push(next_state);
            }
        }
    }

    // Search and find matches.
    vector<vi> positions(patterns.size(), vi());

    int current_state = 0;
    for (int i = 0; i < (int)text.size(); ++i) {
        int c = text[i];

        // Find next state.
        int next = current_state;
        while (delta[next][c] == -1) {
            next = fall[next];
        }
        current_state = delta[next][c];

        // Patterns which are matching at this state.
        for (int pattern : found[current_state]) {
            positions[pattern].push_back(i - patterns[pattern].size() + 1);
        }
    }

    return positions;
}


