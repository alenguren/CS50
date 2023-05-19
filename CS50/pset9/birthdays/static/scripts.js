function addPerson() {
  const nameInput = document.getElementById("name-input");
  const dateInput = document.getElementById("date-input");
  const name = nameInput.value.trim();
  const date = dateInput.value;

  const isValidDate = validateDate(date);

  const person = { name, date };
  persons.push(person);
  saveToLocalStorage(persons);
  nameInput.value = "";
  dateInput.value = "";
  renderList();

  swal.fire({
    icon: "success",
    title: "Success!",
    text: `Birthday for ${name} has been added.`,
  });
}

// Edit a person
function editPerson(id) {
  window.location.href = "/edit?id=" + id;
}

// Delete a person
function deletePerson(id) {
  Swal.fire({
    title: "Are you sure?",
    text: "You won't be able to revert this!",
    icon: "warning",
    showCancelButton: true,
    confirmButtonText: "Yes, delete it!",
    cancelButtonText: "Cancel",
  }).then((result) => {
    if (result.isConfirmed) {
      // Send a POST request to delete the person
      fetch("/delete", {
        method: "POST",
        body: JSON.stringify({ id: id }),
        headers: { "Content-Type": "application/json" },
      })
      .then((response) => {
        if (response.ok) {
          // Reload the page
          location.reload();
        } else {
          Swal.fire({
            title: "Error",
            text: "An error occurred while deleting the person.",
            icon: "error",
          });
        }
      })
      .catch((error) => {
        Swal.fire({
          title: "Error",
          text: "An error occurred while deleting the person.",
          icon: "error",
        });
      });

      // Remove the row from the HTML table without reloading the page
      let row = document.querySelector(`#row-${id}`);
      if (row) {
        row.parentNode.removeChild(row);
      }
    }
  });
}