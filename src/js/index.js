
let load = document.getElementById('load')

window.addEventListener('load', () => {
   setTimeout(() => {
    load.style.display = 'none'

   }, 2000)
  
})
let menu = document.getElementById('menu');
let menu_movile = document.getElementById('menu_movil')

menu.addEventListener('click', () => {
    menu_movile.style.display == 'flex' ? menu_movile.style.display = 'none' :menu_movile.style.display= 'flex'
})