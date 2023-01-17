import java.util.*

/** Class used for parsing MaxWSAT instances from stdin */
class Parser {
    // Parser properties, should be set in first phase
    private var varCount: Int? = null
    private var clauseCount: Int? = null

    // Parser properties, should be set in second phase
    private var weights: List<Int>? = null
    private var line: String? = null

    /** Helper function to load integer list ended by 0 */
    private fun loadIntList(input: String): List<Int> {
        val scanner = Scanner(input) // initialize scanning
        val list: MutableList<Int> = mutableListOf()

        // Load integers as song as we can (or until 0)
        while (scanner.hasNextInt()) {
            val num = scanner.nextInt()
            if (num == 0) break
            list.add(num) // add loaded integers
        }

        // string should be emptied by end of scanning
        if (scanner.hasNext())
            throw ParserException("Line not empty after scanning: ${scanner.next()}")

        return list
    }

    /** Helper function for parsing initial lines of code */
    private fun loadWeightsAndClauseCount() {
        while (true) {
            val ln = readlnOrNull() ?: break // load line
            when (ln.firstOrNull()) { // switch based on first character
                'c' -> continue // skip line
                // instance definition => set properties
                'p' -> {
                    val matches = Regex("p mwcnf (\\d*) (\\d*)").matchEntire(ln)
                    val matchResult = matches?.groupValues ?: throw ParserException("Invalid instance definition")
                    varCount = matchResult[1].toIntOrNull() ?: throw ParserException("Variable count is not a number")
                    clauseCount = matchResult[2].toIntOrNull() ?: throw ParserException("Clause count is not a number")
                }
                // Load weights
                'w' -> {
                    weights = loadIntList(ln.substring(1))
                }
                // End for parsing
                else -> {
                    line = ln
                    break
                }
            }
        }
    }

    /** Helper function for checking parsed values */
    private fun checkWeightsAndVarCount() {
        val weights = weights ?: throw ParserException("Weights not loaded")
        val varCount = varCount ?: throw ParserException("Variable count not loaded")
        if (weights.size != varCount)
            throw ParserException("Weights count does not match variable count")
    }

    /** Helper function to load MaxWSAT clauses */
    private fun loadClauses(): List<ClauseSAT> {
        val clauseCount = clauseCount ?: throw ParserException("Clause count not loaded")
        return List(clauseCount) {
            // Lambda for initializing each line
            val ln = line ?: throw ParserException("Expected clause definition, got EOF")
            val clauses = loadIntList(ln).map { it.toVariable() } // map clauses
            // check clauses
            clauses.forEach {
                if (it.variable <= 0 || it.variable > varCount!!)
                    throw ParserException("Invalid variable ${it.variable} in range 1..$varCount")
            }
            // read next clause line
            line = readlnOrNull()
            return@List ClauseSAT(clauses)
        }
    }

    /** Helper function to load MaxWSAT instance */
    private fun loadInstance() : MaxWSAT {
        loadWeightsAndClauseCount()
        checkWeightsAndVarCount()
        return MaxWSAT(loadClauses(), weights!!)
    }

    companion object {
        /** Static methods for parsing */
        fun parseInstance() = Parser().loadInstance()

        /** Helper method for parsing configuration parameters */
        fun parseConfiguration(args: Array<String>) = Configuration(
            args.getOrNull(0)?.toDoubleOrNull() ?: throw ParserException("Missing initial temperature"),
            args.getOrNull(1)?.toDoubleOrNull() ?: throw ParserException("Missing cooling coefficient"),
            args.getOrNull(2)?.toDoubleOrNull() ?: throw ParserException("Missing weight penalty"),
            args.getOrNull(3)?.toIntOrNull() ?: throw ParserException("Missing equilibrium length"),
            args.getOrNull(4)?.toIntOrNull() ?: 1,
            args.getOrNull(5)?.toIntOrNull() ?: 1,
            args.getOrNull(6)?.toDoubleOrNull(),
            args.getOrElse(7) { "10000" }.toIntOrNull()
        )
    }

    /** Helper parser exception class */
    internal class ParserException(message: String) : RuntimeException("Could not parse instance: $message")
}
