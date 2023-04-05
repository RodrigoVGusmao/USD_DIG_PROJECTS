package janelas;

import org.eclipse.swt.events.PaintEvent;
import org.eclipse.swt.events.PaintListener;
import org.eclipse.swt.graphics.GC;
import org.eclipse.swt.graphics.LineAttributes;
import org.eclipse.swt.graphics.Point;
import org.eclipse.swt.widgets.Label;

import velha.Velha;

public class PaintListenerVelha implements PaintListener {

	private Velha velha;
	private int index;
	
	public PaintListenerVelha(Velha velha, int index)
	{
		super();
		this.velha = velha;
		this.index = index;
	}
	
	@Override
	public void paintControl(PaintEvent e) {
		// TODO Auto-generated method stub
		Label lbl = ((Label) e.getSource());
		Point pt = lbl.getSize();
		float sizeRel = (pt.x+pt.y)/2;
		
		switch(velha.retornarIndex(index))
		{
		case 1:
			drawX(e.gc, pt, Math.max(2*(int)sizeRel/100, 2), Math.max(2*(int)sizeRel/100, 2), Math.max(sizeRel/100, 1.f));
			break;
		case 2:
			drawO(e.gc, pt, Math.max(2*(int)sizeRel/100, 2), Math.max(2*(int)sizeRel/100, 2), Math.max(sizeRel/100, 1.f));
			break;
		}
	}
	
	private void drawX(GC gc, Point pt, int x, int y, float width)
	{
		gc.setLineAttributes(new LineAttributes(width));
		gc.drawLine(x, y, pt.x-x, pt.y-y);
		gc.drawLine(pt.x-x, y, x, pt.y-y);
	}
	
	private void drawO(GC gc, Point pt, int x, int y, float width)
	{
		gc.setLineAttributes(new LineAttributes(width));
		gc.drawOval(x, y, pt.x-x-10, pt.y-y-10);
	}
}