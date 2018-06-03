$("#form").on("submit", (a) => {
  a.preventDefault();
  var data = $("#form").serialize();
  console.log("Form submited", data);
  var http = $.post("/register",
    data,
    (a) => {
      console.log(a);
      if (a.success == true) {
        hide("panel-danger");
        show("panel-success");
        console.log("Succeded");
      } else {
        show("panel-danger");
        hide("panel-success");
        console.log("Failed");
      }
      $("#answer").html('<p>' + a.message + '</p>');
      $('#myModal').show();
    });

  http.fail(() => {
    show("panel-danger");

    $("#answer").html('<p>Something went wrong</p>');
    $('#myModal').show();
    console.log("Error" + a);
    console.log(a);
  });
  return false;
});