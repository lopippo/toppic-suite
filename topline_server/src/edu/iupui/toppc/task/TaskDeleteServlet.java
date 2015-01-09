package edu.iupui.toppc.task;

import java.io.File;
import java.io.IOException;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.commons.io.FileUtils;

import edu.iupui.toppc.util.TaskList;

public class TaskDeleteServlet extends HttpServlet {
	private static final long serialVersionUID = 1L;

	public TaskDeleteServlet() {
		super();
		// TODO Auto-generated constructor stub
	}

	protected void doPost(HttpServletRequest request,
			HttpServletResponse response) throws ServletException, IOException {

		String id = request.getParameter("del");
		String basePath = getServletContext().getRealPath("");
		TaskList.delTaskList(id, basePath + File.separator + "log"
				+ File.separator + "tasklist.xml");
		File zipFile = new File(basePath + File.separator + "download"
				+ File.separator + "TASK" + id + ".zip");
		if (zipFile.exists())
			zipFile.delete();

		FileUtils.deleteDirectory(new File(basePath + File.separator
				+ "process" + File.separator + "TASK" + id));
	}

}
