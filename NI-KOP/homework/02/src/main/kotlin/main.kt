
fun main(args: Array<String>) {
    // Load instance
    val instance = Parser.parseInstance()
    // println(instance)

    // Load configuration
    val configuration = Parser.parseConfiguration(args)
    // println(configuration)

    // Load solution
    val solution = Solver.solve(instance, configuration)
    println(solution.solutionString())
    System.err.println(solution.toString())
}
