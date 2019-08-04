$(document).ready(function () {

    $(".entity").hover(function () {
        $(this).prev().prev().removeClass("col-5").addClass("col");
        $(this).removeClass("col-5").addClass("col-10");
    },
    function () {
        $(this).prev().prev().removeClass("col").addClass("col-5");
        $(this).removeClass("col-10").addClass("col-5");
    });

});
