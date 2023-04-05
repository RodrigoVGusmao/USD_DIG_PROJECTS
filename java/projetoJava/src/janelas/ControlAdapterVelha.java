package janelas;

import org.eclipse.swt.events.ControlAdapter;
import org.eclipse.swt.events.ControlEvent;
import org.eclipse.swt.widgets.Label;

public class ControlAdapterVelha extends ControlAdapter {
	@Override
	public void controlResized(ControlEvent e) {
		((Label) e.getSource()).redraw();
	}
}
