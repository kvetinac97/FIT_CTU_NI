import kotlin.math.exp
import kotlin.random.Random

/** Helper repeat function */
fun repeatUntil(times: Int, action: (Int) -> Unit)
    = repeat(times - 1, action)

/** Class used for solving MaxWSAT instances */
class Solver(private val instance: MaxWSAT, private val configuration: Configuration) {
    // Simulated annealing properties
    private var state = randomState() // initial state = random
    private var finalState: State? = null

    private var temperature = configuration.initialTemperature
    private var lastImprovement = 0
    private var iteration = 0

    /** Function to determine when to stop simulated annealing */
    private fun frozenSA()
        = (configuration.stopIterations == null || iteration > configuration.stopIterations + lastImprovement) &&
          (configuration.stopTemperature == null || temperature <= configuration.stopTemperature)

    /** Function to set next configuration (neighbour) in SA */
    private fun trySA(state: State): State {
        val toFlip = 1 + Random.nextInt(instance.varCount)
        val newState = flip(state.values, toFlip).let { State(it, cost(it)) }
        if (newState.cost > state.cost)
            return newState // better, accept

        val acceptThoughWorse = Random.nextDouble() < exp((newState.cost - state.cost) / temperature)
        return if (acceptThoughWorse) newState else state
    }

    /** Main simulated annealing solve construction */
    private fun solveInstanceSA(): Solution {
        val satisfiedInitial = satisfiedCount(state.values)

        while (!frozenSA()) {
            repeat(configuration.equilibriumLength) {
                val newState = trySA(state)
                if (newState.cost > state.cost) lastImprovement = iteration
                state = newState

                if (satisfiable(state.values) && state.cost > (finalState?.cost ?: 0.0))
                    finalState = state
                iteration += 1
            }
            temperature *= configuration.coolingCoefficient
        }

        val finalState = finalState ?: state
        return Solution(finalState, iteration, lastImprovement, satisfiedCount(finalState.values),
            instance.clauseCount, satisfiedInitial, weight(finalState.values))
    }

    // === SIMULATED ANNEALING END

    /** Helper function to calculate configuration weight */
    private fun weight(values: List<Boolean>) = values.mapIndexed {
            i, it -> if (!it) instance.weights[i] else 0
    }.sum()

    /** Helper function to calculate satisfied formulas in configuration */
    private fun satisfiedCount(values: List<Boolean>) = instance.clauses.sumOf {
        if (it.variables.firstOrNull { v -> values[v.variable - 1] == v.negated } != null) 1
        else (0.0).toInt()
    }

    /** Helper function to check whether configuration is satisfiable */
    private fun satisfiable(values: List<Boolean>)
        = satisfiedCount(values) == instance.clauseCount

    /** Helper function to calculate configuration cost */
    private fun cost(values: List<Boolean>): Double {
        val satisfiedClauses = satisfiedCount(values)
        val weight = weight(values)
        return configuration.weightPenalty * weight + satisfiedClauses
    }

    /** Function to generate random state */
    private fun randomState()
        = List(instance.varCount) { Random.nextBoolean() }.let { State(it, cost(it)) }

    /** Helper function to flip i-th variable in configuration */
    private fun flip(values: List<Boolean>, i: Int)
        = values.mapIndexed { ix, it -> if (i == ix) it.not() else it }

    // === HELPER FUNCTIONS END

    companion object {
        /** Static method for solving */
        fun solve(instance: MaxWSAT, configuration: Configuration): Solution {
            var solution = Solver(instance, configuration).solveInstanceSA()
            var best: Solution? = null
            repeat(configuration.maxRuns) {
                repeatUntil(configuration.maxTries - 1) {
                    if (solution.satisfiedClauses == solution.totalClauses) {
                        if (solution.solutionWeight > (best?.solutionWeight ?: 0))
                            best = solution
                        return@repeatUntil
                    }
                    solution = Solver(instance, configuration).solveInstanceSA()
                }
            }
            return best ?: solution
        }
    }
}
