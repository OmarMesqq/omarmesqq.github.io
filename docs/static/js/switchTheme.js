function loadcss() {

	if (window.matchMedia('(prefers-color-scheme: dark)').matches) {

		var fileref = document.createElement("link");
		fileref.rel = "stylesheet";
		fileref.type = "text/css";
		fileref.href = "../css/dark.css";
		document.getElementsByTagName("head")[0].appendChild(fileref)


	}

	else {

		var fileref = document.createElement("link");
		fileref.rel = "stylesheet";
		fileref.type = "text/css";
		fileref.href = "../css/light.css";
		document.getElementsByTagName("head")[0].appendChild(fileref)

	}


}

