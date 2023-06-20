function scrollToSection(sectionId, offset) {
  var section = document.getElementById(sectionId);
  var sectionPosition = section.offsetTop - offset;
  window.scrollTo({
    top: sectionPosition,
    behavior: 'smooth'
  });
}