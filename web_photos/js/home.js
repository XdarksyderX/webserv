"use strict";

const API_KEY = "MY_API_KEY_PIXELS"; // my email
const searchForm = document.querySelector(".search-form");
const imageGallery = document.querySelector("#image-gallery");
const searchInput = document.querySelector("#search-input");

// Function to fetch and display images
function getImages(searchTerm) {
	const baseURL = `https://api.pexels.com/v1/search?query=${searchTerm}&per_page=12`;

	fetch(baseURL, {
		method: "GET",
		headers: {
			Authorization: API_KEY,
		},
	})
		.then((response) => response.json())
		.then((data) => {
			displayImages(data.photos.slice(0, 3)); // Limit to first 4 images
		})
		.catch((error) => console.error(error));
}

function changeTitle(searchName) {
	const h2 = document.querySelector("h2");
	if (searchName.length !== 0) h2.textContent = searchName;
}

// Function to display images
function displayImages(photos) {
	const body = document.querySelector("body");
	body.style.background = `url(${photos[0].src.original})`; // Change background img

	imageGallery.innerHTML = ""; // Clear previous images
	photos.forEach((photo) => {
		const imageElement = document.createElement("img"); // popular photos
		imageElement.src = photo.src.medium;
		imageElement.alt = photo.photographer;
		imageGallery.appendChild(imageElement);
	});
}

// Event listener for search form submission
searchForm.addEventListener("submit", (e) => {
	e.preventDefault();
	getImages(searchInput.value);
	changeTitle(searchInput.value);
});
// zT4rj69XUx6yKVckyztD0GsNEo11j07eLEsR0cq7MWE7immczDwUxEnL
