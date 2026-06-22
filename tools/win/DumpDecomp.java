import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import java.io.*;
public class DumpDecomp extends GhidraScript {
    public void run() throws Exception {
        PrintWriter out = new PrintWriter(new FileWriter("/tmp/eets_decomp.c"));
        DecompInterface di = new DecompInterface();
        di.openProgram(currentProgram);
        int n = 0;
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
        while (it.hasNext()) {
            Function f = it.next();
            try {
                DecompileResults r = di.decompileFunction(f, 20, monitor);
                DecompiledFunction dc = (r != null) ? r.getDecompiledFunction() : null;
                if (dc != null) { out.println("//FUNC " + f.getEntryPoint()); out.print(dc.getC()); n++; }
            } catch (Exception e) {}
        }
        out.close();
        println("DONE export " + n);
    }
}
