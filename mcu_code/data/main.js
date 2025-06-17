
document.addEventListener('DOMContentLoaded', function() {
  var container = document.querySelector('.container');
  if (container) {
    container.addEventListener('click', function(e) {
      var link = e.target.classList.contains('overlay-link');
      if (link) {
        document.getElementById('pageOverlay').style.display = 'none';
        setTimeout(function() {
          document.getElementById('pageOverlay').style.display = 'flex';
        }, 200);
      }
    });
  }
});