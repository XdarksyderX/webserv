"use strict";

const form = document.querySelector("form");

form.addEventListener("submit", (e) => {
	e.preventDefault();
	const totalDOM = document.querySelector(".total");

	let continente = document.getElementsByName("continente")[0];
	continente = continente.value;
	while (
		continente !== "America" &&
		continente !== "Asia" &&
		continente !== "Europa" &&
		continente !== "Africa" &&
		continente !== "Oceania"
	)
		return;

	const n_dias = document.getElementsByName("n_dias")[0];
	while (n_dias.value <= 0) return;
	let priceLocation = 0;
	if (continente == "America") priceLocation = 1;
	else if (continente == "Asia") priceLocation = 2;
	else if (continente == "Europa") priceLocation = 3;
	else if (continente == "Africa") priceLocation = 4;
	else if (continente == "Oceania") priceLocation = 5;
	const total = parseInt(n_dias.value) * priceLocation;
	totalDOM.innerHTML = `<h2>Total: ${total}€ </h2>`;
});
