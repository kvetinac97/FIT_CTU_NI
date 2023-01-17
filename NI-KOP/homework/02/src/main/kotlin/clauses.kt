/** Class representing one variable */
data class VariableSAT(val variable: Int, val negated: Boolean) {
    override fun toString() = "${if (negated) "-" else ""}$variable"
}

/** Helper extension function to convert Int into Variable */
fun Int.toVariable() = if (this < 0) VariableSAT(-this, true)
                       else VariableSAT(this, false)

/** Class representing one SAT clause (of any size) */
data class ClauseSAT(val variables: List<VariableSAT>) {
    override fun toString() = variables.joinToString(" v ")
}

/** Class representing one MaxWSAT instance */
data class MaxWSAT(val clauses: List<ClauseSAT>, val weights: List<Int>) {
    val varCount: Int = weights.size
    val clauseCount: Int = clauses.size
    override fun toString() = "clauses=${clauses.joinToString(" ^ ") { "(${it})" }}\n" +
            "weights=$weights"
}

// === INSTANCE DEFINITION END

/** Helper class for holding configuration properties */
data class Configuration(val initialTemperature: Double, val coolingCoefficient: Double,
                         val weightPenalty: Double, val equilibriumLength: Int,
                         val maxTries: Int = 1, val maxRuns: Int = 1,
                         val stopTemperature: Double? = null, val stopIterations: Int? = 10000) {
    override fun toString() = "{T=$initialTemperature, alpha=$coolingCoefficient, wp=$weightPenalty, " +
            "N=$equilibriumLength, maxTries=$maxTries, maxRuns=$maxRuns, " +
            "T_final=$stopTemperature, it_max=$stopIterations}"
}

/**
 * Internal algorithm state (variable values)
 * contains `cost` attribute for faster cost calculation
 */
data class State(val values: List<Boolean>, val cost: Double) {
    override fun toString() = values.mapIndexed { i, it -> "${if (it) "-" else ""}${i+1}" }.toString()
}

/** Helper function for solution printing */
fun State.solutionString()
    = values.mapIndexed { i, it -> "${if (it) "-" else ""}${i+1}" }
            .joinToString(" ", "", " 0")

/** Class representing algorithm result */
data class Solution(val finalState: State, val totalIterations: Int, val lastImprovement: Int,
                  val satisfiedClauses: Int, val totalClauses: Int,
                  val satisfiedInitial: Int, val solutionWeight: Int) {
    fun solutionString() = "$solutionWeight ${finalState.solutionString()}"

    override fun toString() = "$totalIterations $lastImprovement $satisfiedClauses " +
                              "$totalClauses $satisfiedInitial $solutionWeight"
}

// === SOLUTION DEFINITION END
