$(document).ready(function() {
  // Set the active link in the navbar based on the current page
  var path = window.location.pathname;
  $('.navbar-nav > li > a[href="'+path+'"]').addClass('active');

  // Add smooth scrolling to anchor links
  $('a[href*="#"]').not('[href="#"]').not('[href="#0"]').click(function(event) {
    if (location.pathname.replace(/^\//, '') == this.pathname.replace(/^\//, '') && location.hostname == this.hostname) {
      var target = $(this.hash);
      target = target.length ? target : $('[name=' + this.hash.slice(1) + ']');
      if (target.length) {
        event.preventDefault();
        $('html, body').animate({
          scrollTop: target.offset().top
        }, 1000, function() {
          var $target = $(target);
          $target.focus();
          if ($target.is(":focus")) {
            return false;
          } else {
            $target.attr('tabindex','-1');
            $target.focus();
          };
        });
      }
  });

  // Toggle the navbar when the button is clicked
  $('.toggle-button').click(function(){
    $('.navbar').toggleClass('open');
    // Add this line to close the navbar when the toggler button is clicked
    $('.navbar').removeClass('open');
  });
});
