javascript:(function () {
  function copyToClipboard(text) {
    navigator.clipboard.writeText(text).then(function() {
      alert("Video timecode copied to clipboard: " + text);
    }, function(err) {
      alert("Could not copy text: ", err);
    });
  }

  try {
    const video = document.querySelector("video");
    if (video) {
      const timecode = video.currentTime;
      copyToClipboard(timecode);
    } else {
      alert("No video element found on this page.");
    }
  } catch (error) {
    alert("An error occurred: " + error.message);
  }
})();
