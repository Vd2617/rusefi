import com.rusefi.NamedThreadFactory;
import com.rusefi.SerialPortScanner;
import com.rusefi.core.rusEFIVersion;
import com.rusefi.maintenance.jobs.AsyncJobExecutor;
import com.rusefi.maintenance.jobs.DfuManualJob;
import com.rusefi.maintenance.jobs.OpenBltManualJob;
import com.rusefi.ui.StatusWindow;
import com.rusefi.ui.widgets.ToolButtons;

import javax.swing.*;
import java.lang.reflect.InvocationTargetException;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Set;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.stream.Collectors;

import static com.rusefi.SerialPortScanner.SerialPortType.OpenBlt;

public class MassUpdater {
    private final StatusWindow mainStatus = new StatusWindow();
    private final Set<String> knownBlts = new HashSet<>();

    public MassUpdater() {
        mainStatus.showFrame("Mass Updater " + rusEFIVersion.CONSOLE_VERSION);

        final AtomicBoolean previousDfuState = new AtomicBoolean();

        SerialPortScanner.INSTANCE.addListener(currentHardware -> {

            if (!previousDfuState.get() && currentHardware.isDfuFound()) {
                SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJob(new DfuManualJob()));
            }
            previousDfuState.set(currentHardware.isDfuFound());

            List<SerialPortScanner.PortResult> currentBltList = currentHardware.getKnownPorts().stream().filter(portResult -> portResult.type == OpenBlt).collect(Collectors.toList());
            Set<String> currentSet = currentBltList.stream().map(portResult -> portResult.port).collect(Collectors.toSet());
            for (Iterator<String> it = knownBlts.iterator(); it.hasNext(); ) {
                String port = it.next();
                if (!currentSet.contains(port)) {
                    mainStatus.append(port + ": No longer present");
                    it.remove();
                }
            }
            for (SerialPortScanner.PortResult openBltPort : currentBltList) {
                if (!knownBlts.contains(openBltPort.port)) {
                    knownBlts.add(openBltPort.port);
                    mainStatus.append("New OpenBlt " + openBltPort);

                    SwingUtilities.invokeLater(() -> AsyncJobExecutor.INSTANCE.executeJob(new OpenBltManualJob(openBltPort, mainStatus.getContent())));
                }
            }
        });
    }

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        ToolButtons.showDeviceManager();
        SwingUtilities.invokeAndWait(MassUpdater::new);
    }
}
