
document.addEventListener('DOMContentLoaded', function() {
  var playing = "{{PLAYING}}" === "true";
  var toggle = document.getElementById('playingToggle');
  var label = document.getElementById('toggleLabel');
  if (toggle) toggle.checked = playing;
  if (label) label.textContent = playing ? "Playing" : "Stopped";

  var container = document.querySelector('.container');
  if (container) {
    container.addEventListener('click', function(e) {
      var link = e.target.closest('.overlay-link');
      if (link) {
        setTimeout(function() {
          document.getElementById('pageOverlay').style.display = 'flex';
        }, 200);      }
    });
  }
});