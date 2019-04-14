function update(full){
	fetch('./status').then(function(response) {
	  return response.json();
	}).then(function(myJson) {
		var tbl = document.getElementById('maintable');
		var frm = document.getElementById('mainform');
		tblJson = myJson.table;
		formJson = myJson.form	;

		keys = Object.keys(tblJson);
		tbl.innerHTML = '';
		for (i = 0; i < keys.length; i++){
			key = keys[i];
			value = tblJson[key];
			row = tbl.insertRow(-1);
			cell = row.insertCell(-1);
			cell.innerHTML = key;
			cell = row.insertCell(-1);
			cell.innerHTML = value;
		}

		c = "";
		if (parseInt(tblJson.analog_value) > parseInt(tblJson.analog_threshold)){
			c = "filter: saturate(0);";
		}
		tbl.innerHTML += `<meter min=0 max=1024 value=${tblJson.analog_value} style='${c}'></meter>`;
		
		if (full){
			document.getElementById("analog_thresholdInput").value = tblJson.analog_threshold;
			keys = Object.keys(formJson);
			frm.innerHTML = "";
			for (i = 0; i < keys.length; i++){
				key = keys[i];
				value = formJson[key];
				frm.innerHTML += `<label><input name='${key}' value='${value}' required><span class='desc'>${key.replace("_", " ")}</span></label>`;
			}
			frm.innerHTML += `<button>Save</button>`;
		}
	});
}

update(true);
setInterval(function(){update(false)}, 400);