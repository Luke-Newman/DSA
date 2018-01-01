//
// Created by luken on 2/09/2017.
//

#ifndef SLIDING_PUZZLE_V2_SLIDING_PUZZLE_H
#define SLIDING_PUZZLE_V2_SLIDING_PUZZLE_H

#include <chrono>
#include <utility>
#include <vector>
#include <list>
#include <queue>
#include <random>
#include <algorithm>
#include <memory>

/** CPP 11 **/

static int deleted = 0;

unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

class State
{
public:
    std::vector<int> state;
    std::shared_ptr<State> predecessor;
    int path_cost, total_cost = 0; /* total cost for A* -> f = g + h */

    State(std::vector<int> state, int path_cost) :
            state(std::move(state)), path_cost(path_cost) {} /* used for initial */

    State(std::vector<int> state, int p_cost, int t_cost, std::shared_ptr<State> s) :
            state(std::move(state)), path_cost(p_cost), total_cost(t_cost), predecessor(s) {}

    virtual ~State() { deleted++; }

    bool operator==(std::vector<int> s)
    {
        for (int i = 0; i < state.size(); i++)
        {
            if (state[i] != s[i]) return false;
        }
        return true;
    }

};

/* A* functor for comparing the heuristic + path cost */
struct compareHeuristic : public std::binary_function<std::shared_ptr<State>, std::shared_ptr<State>, bool>
{
    bool operator()(const std::shared_ptr<State> &lhs, const std::shared_ptr<State> &rhs) const
    {
        return lhs->total_cost > rhs->total_cost;
    }
};

/* BFS functor for comparing path costs */
struct comparePath : public std::binary_function<std::shared_ptr<State>, std::shared_ptr<State>, bool>
{
    bool operator()(const std::shared_ptr<State> &lhs, const std::shared_ptr<State> &rhs) const
    {
        return lhs->path_cost > rhs->path_cost;
    }
};

class Sliding_puzzle
{

private:

public:
    Sliding_puzzle() = default;

    std::vector<int> generate_initial();
    void print_state(std::vector<int> state);
    bool goal_achieved(std::vector<int> state);
    std::vector< std::pair<std::vector<int>, int> > generate_actions(std::vector<int> &state, int space_index);
    int get_space_index(std::vector<int> &state);
    void breadth_first_search(std::vector<int> initial_state);
    void a_star_search(std::vector<int> initial_state);
    int get_heuristic(const std::vector<int> &s, int space_index);
    bool move_tried(std::vector<std::vector<int> > &tried_moves, std::vector<int> &current);
    void get_path(std::shared_ptr<State> state);

};

std::vector<int> Sliding_puzzle::generate_initial()
{
    std::vector<int> board {1, 1, 1, -1, -1, -1, 0};
    std::shuffle(board.begin(), board.end(), std::default_random_engine(seed));

    return board;
}

void Sliding_puzzle::print_state(std::vector<int> state) {
    for (int i = 0; i < state.size(); i++)
    {
        std::cout << " ---";
    }

    std::cout << "\n|";

    for (int i : state)
    {
        if (i < 0)
        {
            std::cout << " B |";
        }
        else if (i > 0)
        {
            std::cout << " W |";
        }
        else
        {
            std::cout << "   |";
        }
    }

    std::cout << "\n";

    for (int i = 0; i < state.size(); i++)
    {
        std::cout << " ---";
    }

    std::cout << "\n";
}

bool Sliding_puzzle::goal_achieved(std::vector<int> state)
{
    int num_white = state[0] + state[1] + state[2];

    if (num_white == 3)
        return true;

    num_white += state[3];

    return num_white == 3;
}

std::vector< std::pair<std::vector<int>, int> >
Sliding_puzzle::generate_actions(std::vector<int> &state, int space_index)
{
    std::vector< std::pair<std::vector<int>, int> > available_actions;
    std::vector<int> tmpState;
    int max_jumps_rhs = space_index + 3;
    int max_jumps_lhs = space_index - 3;
    int path_cost = 0;

    /* available swaps from rhs of index */
    for (int i = space_index+1; i <= max_jumps_rhs; i++)
    {
        if (i >= state.size()) break;

        tmpState = state;

        if (i == max_jumps_rhs)
            path_cost = 2;
        else
            path_cost = 1;

        std::swap(tmpState[space_index], tmpState[i]);
        available_actions.emplace_back(tmpState, path_cost);
    }

    /* available swaps from lhs of index */
    for (int i = space_index-1; i >= 0; i--)
    {
        if (i < 0 || i < max_jumps_lhs) break;

        tmpState = state;

        if (i == max_jumps_lhs)
            path_cost = 2;
        else
            path_cost = 1;

        std::swap(tmpState[space_index], tmpState[i]);
        available_actions.emplace_back(tmpState, path_cost);
    }

    return available_actions;
}

int Sliding_puzzle::get_space_index(std::vector<int> &state)
{
    for (int i = 0; i < state.size(); i++)
    {
        if (state[i] == 0)
            return i;
    }

    std::cout << "error finding white space in state\n";
    return 0;
}

int Sliding_puzzle::get_heuristic(const std::vector<int> &s, int space_index)
{
    int val_1 = 0, val_2 = 0;

    for (int i = 0; i < s.size(); i++)
    {

        /* Checking both W and B tiles together seems more effective */
        if (s[i] > 0)
            val_1 += i;

        if (s[i] < 0)
            val_2 += (7 - i);

    }

    return val_1 + val_2 + abs(space_index-3);
}

void Sliding_puzzle::breadth_first_search(std::vector<int> initial_state)
{
    std::priority_queue<std::shared_ptr<State>, std::vector<std::shared_ptr<State> >, comparePath> solution_queue;
    std::vector<std::vector<int>> tried_moves; /* check an action against all tried states */
    std::vector< std::pair<std::vector<int>, int> > actions;
    int space_index = 0, count = 0, total_cost = 0;

    solution_queue.emplace(std::make_shared<State>(initial_state,0));
    tried_moves.push_back(initial_state);

    while(!solution_queue.empty())
    {
        std::shared_ptr<State> current_state(solution_queue.top());
        solution_queue.pop();
        count ++;

        if (goal_achieved(current_state->state))
        {
            std::cout << "\nGoal achieved after checking " << count <<" states:\n";
            print_state(current_state->state);

            get_path(current_state);
            return;
        }

        space_index = get_space_index(current_state->state);
        actions = generate_actions(current_state->state, space_index);

        for (auto s : actions)
        {
            if (!move_tried(tried_moves, s.first))
            {
                tried_moves.push_back(s.first);
                total_cost = current_state->path_cost + s.second;
                solution_queue.emplace(std::make_shared<State>(s.first, total_cost, 0, current_state));

            }
        }
    }
}

void Sliding_puzzle::a_star_search(std::vector<int> initial_state) {
    std::priority_queue<std::shared_ptr<State>, std::vector<std::shared_ptr<State> >, compareHeuristic> solution_queue;
    std::vector<std::vector<int>> tried_moves;
    std::vector< std::pair<std::vector<int>, int> > actions;
    int space_index = 0, count = 0, path_cost = 0, total_cost = 0;

    solution_queue.emplace(std::make_shared<State>(initial_state,0));
    tried_moves.push_back(initial_state);

    while (!solution_queue.empty())
    {
        std::shared_ptr<State> current_state(solution_queue.top());
        solution_queue.pop();
        count ++;

        if (goal_achieved(current_state->state))
        {
            std::cout << "\nGoal achieved after checking " << count <<" states:\n";
            print_state(current_state->state);

            get_path(current_state);
            return;
        }

        space_index = get_space_index(current_state->state);
        actions = generate_actions(current_state->state, space_index);

        for (auto s : actions)
        {
            if (!move_tried(tried_moves, s.first))
            {
                tried_moves.push_back(s.first);
                // if the move is not tried get heuristic here
                path_cost = current_state->path_cost + s.second;
                total_cost = path_cost + get_heuristic(s.first, space_index); /* f = g + h */
                solution_queue.emplace(std::make_shared<State>(s.first, path_cost, total_cost, current_state));
            }
        }
    }
}

/* checking if an action has been tried and is in the closed set */
bool Sliding_puzzle::move_tried(std::vector<std::vector<int> > &tried_moves, std::vector<int> &current)
{
    for (const std::vector<int> &s : tried_moves)
    {
        if (current == s) return true;
    }
    return false;
}

/* iterating from goal to initial via predecessor and printing the state */
void Sliding_puzzle::get_path(std::shared_ptr<State> state)
{
    std::list<std::shared_ptr<State>> path;
    std::shared_ptr<State> current_state = std::move(state);
    int total_path_cost = 0;

    while (current_state->predecessor != nullptr)
    {
        total_path_cost += current_state->path_cost;
        path.push_front(current_state);
        current_state = current_state->predecessor;
    }

    std::cout << "\nSolution Path:\n";

    for (auto &i : path)
    {
        print_state(i->state);
        std::cout << "Path cost: " << i->path_cost << ", Heuristic: " << i->total_cost << "\n\n";
    }

    std::cout << "Total path cost: " << total_path_cost <<std::endl;
}

#endif //SLIDING_PUZZLE_V2_SLIDING_PUZZLE_H
