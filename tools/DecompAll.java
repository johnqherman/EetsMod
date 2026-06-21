// Ghidra headless script: decompile every function to one C file.
// Usage: -postScript DecompAll.java <output_path>
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import ghidra.program.model.listing.FunctionManager;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.PrintWriter;

public class DecompAll extends GhidraScript {
    public void run() throws Exception {
        String[] args = getScriptArgs();
        String out = (args.length > 0) ? args[0] : "/tmp/eets_decomp.c";

        DecompInterface ifc = new DecompInterface();
        ifc.openProgram(currentProgram);
        ConsoleTaskMonitor monitor = new ConsoleTaskMonitor();
        FunctionManager fm = currentProgram.getFunctionManager();
        int total = fm.getFunctionCount();

        PrintWriter w = new PrintWriter(new BufferedWriter(new FileWriter(out)));
        w.println("// Eets full decompilation (Ghidra) - " + total + " functions");
        w.println("// program: " + currentProgram.getName());
        w.println();

        int done = 0, fail = 0;
        FunctionIterator it = fm.getFunctions(true);
        while (it.hasNext()) {
            Function f = it.next();
            String hdr = f.getName() + " @ " + f.getEntryPoint();
            try {
                DecompileResults r = ifc.decompileFunction(f, 90, monitor);
                if (r != null && r.decompileCompleted()) {
                    w.println("// ======== " + hdr + " ========");
                    w.println(r.getDecompiledFunction().getC());
                    done++;
                } else {
                    w.println("// ======== " + hdr + " (decompile failed) ========");
                    w.println();
                    fail++;
                }
            } catch (Exception e) {
                w.println("// ======== " + hdr + " (error: " + e + ") ========");
                w.println();
                fail++;
            }
            if ((done + fail) % 500 == 0) {
                println("  ... " + (done + fail) + "/" + total);
            }
        }
        w.close();
        println("DecompAll: wrote " + done + " functions (" + fail + " failed) to " + out);
    }
}
