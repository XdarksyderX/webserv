const setCookie = document.getElementById('setCookie');
		setCookie.addEventListener('submit', (event) => {
			event.preventDefault();
			const key = document.getElementById('key').value;
			const value = document.getElementById('value').value;
			localStorage.setItem(key, value);
			alert(`valor de ${key} guardado correctamente`);
		});

		const getCookie = document.getElementById("getCookie");
		getCookie.addEventListener('submit', (event) => {
			event.preventDefault();
			const itemName = document.getElementById('name').value;
			const itemValue = localStorage.getItem(itemName);
			if (itemValue) {
				alert(itemValue);
			} else {
				alert("Cookie not found");
			}
		})
