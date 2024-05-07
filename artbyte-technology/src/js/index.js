
// === Preloader ===
let load = document.getElementById('load')

window.onload = () => {
    load.style.display='none'
}

// === Btn Menu Movile ===
let menu = document.getElementById('menu');
let menu_movile = document.getElementById('menu_movil')

menu.addEventListener('click', () => {
    menu_movile.style.display == 'flex' ? menu_movile.style.display = 'none' :menu_movile.style.display= 'flex'
})