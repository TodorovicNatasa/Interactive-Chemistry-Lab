#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

const int W = 800;
const int H = 600;

sf::RenderWindow window(sf::VideoMode({ W, H }), "Zadatak Z2 - Hemija");

float pulseTimer = 0.f;
float tajmerMjehurica = 0.f;
float brzinaKapljice = 0.f;

const int BROJ_EPRUVETA = 4;
const int BROJ_MALIH_SLIKA = 3;

const float STO_X = 40.f;
const float STO_Y = 450.f;
const float STO_W = 720.f;
const float STO_H = 35.f;

const float GLAVNA_SLIKA_X = W / 2.f - 95.f;
const float GLAVNA_SLIKA_Y = 30.f;
const float GLAVNA_SLIKA_W = 190.f;
const float GLAVNA_SLIKA_H = 150.f;

const float STALAK_X = STO_X + 115.f;
const float STALAK_Y = STO_Y - 160.f;

const float DESTILACIJA_X = STO_X + 370.f;
const float DESTILACIJA_Y = STO_Y - 270.f;

const float MIKROSKOP_X = STO_X + 10.f;
const float MIKROSKOP_Y = STO_Y - 10.f;

const float MIS_Y = STO_Y - 32.f;

const float UPOZORENJE_Y = H - 50.f;

sf::Color TOXIC_ZELENA(120, 255, 60, 230);
sf::Color TOXIC_TAMNA(20, 180, 40, 220);
sf::Color TOXIC_SVIJETLA(180, 255, 140, 200);
sf::Color TOXIC_ZUTA(255, 255, 80, 220);
sf::Color TOXIC_NARANDZASTA(255, 180, 30, 230);

sf::Color RAM_DARK(80, 50, 30);
sf::Color SLIKA_POZADINA(245, 245, 240);
sf::Color HEMIJA_LJUBICASTA(155, 0, 95);
sf::Color HEMIJA_PLAVA(40, 40, 110);
sf::Color HEMIJA_ZELENA(20, 180, 40);
sf::Color HEMIJA_NARANDZASTA(255, 180, 30);
sf::Color ZID_BOJA(15, 25, 15);
sf::Color POD_BOJA(10, 20, 15);

enum NivoTecnosti { POLUPUNA = 1, PUNA = 2 };

struct Pravougaonik {
    float x, y, w, h;
};

bool preklapajuSe(Pravougaonik a, Pravougaonik b) {
    return !(a.x + a.w < b.x || b.x + b.w < a.x ||
        a.y + a.h < b.y || b.y + b.h < a.y);
}
void nacrtajSjenu(float x, float y, float w, float h, float blur = 8.f, sf::Color boja = sf::Color(0, 0, 0, 80)) {
    float vanjskiSirina = w + (blur * 1.5f);
    float vanjskiVisina = h + (blur * 1.5f);

    float unutrasnjiSirina = w + (blur * 0.5f);
    float unutrasnjiVisina = h + (blur * 0.5f);

    float offsetX = blur * 0.375f;
    float offsetY = blur * 0.375f;

    sf::RectangleShape vanjski({ vanjskiSirina, vanjskiVisina });
    vanjski.setPosition({ x - offsetX, y - offsetY });
    vanjski.setFillColor(sf::Color(boja.r, boja.g, boja.b, boja.a / 2));
    window.draw(vanjski);

    sf::RectangleShape unutrasnji({ unutrasnjiSirina, unutrasnjiVisina });
    unutrasnji.setPosition({ x - (offsetX / 3), y - (offsetY / 3) });
    unutrasnji.setFillColor(boja);
    window.draw(unutrasnji);
}

void nacrtajGlow(float cx, float cy, float radius,
    sf::Color boja, int alpha) {
    for (int i = 4; i >= 0; i--) {
        float r = radius + i * 8.f;
        int a = alpha / (i + 1);
        sf::CircleShape glow(r);
        glow.setOrigin({ r, r });
        glow.setPosition({ cx, cy });
        glow.setFillColor(sf::Color(boja.r, boja.g, boja.b, a));
        window.draw(glow);
    }
}

void nacrtajPozadinu() {
  
    sf::RectangleShape Zid({ (float)W, 290.f });
    Zid.setPosition({ 0.f, 0.f });
    Zid.setFillColor(ZID_BOJA);
    window.draw(Zid);

    sf::RectangleShape pod({ (float)W, 310.f });
    pod.setPosition({ 0.f, 290.f });
    pod.setFillColor(POD_BOJA);
    window.draw(pod);

    nacrtajGlow(200.f, 150.f, 60.f, sf::Color(120, 255, 60), 25);
    nacrtajGlow(650.f, 100.f, 50.f, sf::Color(255, 255, 80), 20);
    nacrtajGlow(100.f, 80.f, 40.f, sf::Color(255, 100, 50), 15);
}

void nacrtajUpozorenjeTraku(float y) {
    sf::RectangleShape traka({ 40.f, 12.f });
    traka.setFillColor(sf::Color(255, 255, 80));
    for (int i = 0; i < 20; i += 2) {
        traka.setPosition({ i * 40.f, y });
        window.draw(traka);
    }
    traka.setFillColor(sf::Color(20, 20, 20));
    for (int i = 1; i < 20; i += 2) {
        traka.setPosition({ i * 40.f, y });
        window.draw(traka);
    }
}

void nacrtajSto() {

    for (int i = 0; i < 5; i++) {
        sf::RectangleShape traka({ STO_W, STO_H / 5.f });
        traka.setPosition({ STO_X, STO_Y + i * (STO_H / 5.f) });
        int svjetlina = 50 + i * 8;
        traka.setFillColor(sf::Color(svjetlina, svjetlina + 10, svjetlina + 5));
        window.draw(traka);
    }

    float visinaNoge = 600.f - (STO_Y + STO_H);
    sf::RectangleShape noga({ 35.f, visinaNoge });
    noga.setFillColor(sf::Color(40, 50, 40));
    noga.setPosition({ STO_X + 40.f, STO_Y + STO_H });
    window.draw(noga);
    noga.setPosition({ STO_X + STO_W - 75.f, STO_Y + STO_H });
    window.draw(noga);

    sf::RectangleShape ploca({ STO_W, STO_H });
    ploca.setPosition({ STO_X, STO_Y });
    ploca.setFillColor(sf::Color::Transparent);
    ploca.setOutlineColor(sf::Color(100, 120, 100));
    ploca.setOutlineThickness(2.f);
    window.draw(ploca);

    for (int i = 0; i < 5; i++) {
        sf::RectangleShape linija({ STO_W, 1.f });
        linija.setPosition({ STO_X, STO_Y + 5.f + i * 7.f });
        linija.setFillColor(sf::Color(120, 140, 120, 100));
        window.draw(linija);
    }
}

void nacrtajEpruvetu(float x, float y, int nivo, sf::Color boja) {
    const float sirina = 32.f;
    const float visina = 105.f;
    const float debljinaStakla = 2.f;

    if (nivo != 0) {
        int alpha = 40 + (int)(std::sin(pulseTimer) * 25);  

        for (int i = 2; i >= 0; i--) {
            float r = 26.f + i * 8.f;
            int a = alpha / (i + 1);
            sf::CircleShape glow(r);
            glow.setOrigin({ r, r });
            glow.setPosition({ x + sirina / 2.f, y + visina / 2.f + 10.f });
            glow.setFillColor(sf::Color(boja.r, boja.g, boja.b, a));
            window.draw(glow);
        }
    }

    sf::RectangleShape stakloPravougaonik({ sirina - 2.f * debljinaStakla, visina - 16.f });
    stakloPravougaonik.setPosition({ x + debljinaStakla, y });
    stakloPravougaonik.setFillColor(sf::Color(220, 240, 255, 50));
    window.draw(stakloPravougaonik);

    sf::CircleShape stakloDno((sirina - 2.f * debljinaStakla) / 2.f);
    stakloDno.setPosition({ x + debljinaStakla, y + visina - 16.f - (sirina - 2.f * debljinaStakla) / 2.f });
    stakloDno.setFillColor(sf::Color(220, 240, 255, 50));
    window.draw(stakloDno);

    sf::RectangleShape lijevaLinija({ debljinaStakla + 0.5f, visina - 16.f });
    lijevaLinija.setPosition({ x - 0.25f, y });
    lijevaLinija.setFillColor(sf::Color(255, 255, 255, 200));
    window.draw(lijevaLinija);

    sf::RectangleShape desnaLinija({ debljinaStakla + 0.5f, visina - 16.f });
    desnaLinija.setPosition({ x + sirina - debljinaStakla - 0.25f, y });
    desnaLinija.setFillColor(sf::Color(255, 255, 255, 200));
    window.draw(desnaLinija);

    sf::CircleShape donjiLuk((sirina - 2.f * debljinaStakla) / 2.f);
    donjiLuk.setPosition({ x + debljinaStakla, y + visina - 16.f - (sirina - 2.f * debljinaStakla) / 2.f });
    donjiLuk.setFillColor(sf::Color::Transparent);
    donjiLuk.setOutlineColor(sf::Color(255, 255, 255, 200));
    donjiLuk.setOutlineThickness(debljinaStakla);
    donjiLuk.setPointCount(20);
    window.draw(donjiLuk);

    float procenat = 0.f;
    if (nivo == POLUPUNA) procenat = 0.5f;
    else if (nivo == PUNA) procenat = 0.9f;

    float hTecnosti = visina * procenat;

    if (procenat > 0.f) {

        sf::RectangleShape tecnostPravougaonik({ sirina - 4.f * debljinaStakla, hTecnosti - 16.f });
        tecnostPravougaonik.setPosition({ x + 2.f * debljinaStakla, y + visina - hTecnosti });
        tecnostPravougaonik.setFillColor(boja);
        window.draw(tecnostPravougaonik);

        sf::CircleShape tecnostDno((sirina - 4.f * debljinaStakla) / 2.f);
        tecnostDno.setPosition({ x + 2.f * debljinaStakla, y + visina - 16.f - (sirina - 4.f * debljinaStakla) / 2.f });
        tecnostDno.setFillColor(boja);
        window.draw(tecnostDno);
    }

    sf::RectangleShape cep({ 24.f, 6.f });
    cep.setPosition({ x + 4.f, y - 6.f });
    cep.setFillColor(sf::Color(160, 120, 80));
    cep.setOutlineColor(sf::Color(120, 80, 40));
    cep.setOutlineThickness(1.f);
    window.draw(cep);
    sf::RectangleShape cepSjaj({ 8.f, 2.f });
    cepSjaj.setPosition({ x + 6.f, y - 5.f });
    cepSjaj.setFillColor(sf::Color(220, 180, 140));
    window.draw(cepSjaj);

    sf::RectangleShape refleksija({ 4.f, visina - 30.f });
    refleksija.setPosition({ x + 5.f, y + 8.f });
    refleksija.setFillColor(sf::Color(255, 255, 255, 70));
    window.draw(refleksija);
 
    sf::RectangleShape refleksija2({ 2.f, visina - 50.f });
    refleksija2.setPosition({ x + 10.f, y + 15.f });
    refleksija2.setFillColor(sf::Color(255, 255, 255, 35));
    window.draw(refleksija2);
}

struct Mjehuric {
    float x, y, brzinaX, brzinaY, velicina;
    int zivot;
};

Mjehuric vanjskiMjehuri[4][20];

void inicijalizujMjehurice() {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 20; j++)
            vanjskiMjehuri[i][j].zivot = 0;
}
void dodajMjehuric(int epruvetaIndex, float epX, float epY) {
    for (int j = 0; j < 20; j++) {
        if (vanjskiMjehuri[epruvetaIndex][j].zivot <= 0) {
            vanjskiMjehuri[epruvetaIndex][j].x = epX + 16.0f;
            vanjskiMjehuri[epruvetaIndex][j].y = epY - 5.0f;
            vanjskiMjehuri[epruvetaIndex][j].brzinaX = -0.8f + (std::rand() % 161) / 100.0f;
            vanjskiMjehuri[epruvetaIndex][j].brzinaY = -2.0f + (std::rand() % 151) / 100.0f;
            vanjskiMjehuri[epruvetaIndex][j].velicina = 2.0f + (std::rand() % 31) / 10.0f;
            vanjskiMjehuri[epruvetaIndex][j].zivot = 80 + (std::rand() % 61);   
            return;
        }
    }
}
void azurirajVanjskeMjehurice(float epX, float epY, int epruvetaIndex) {
    if ((std::rand() % 100) < 8) dodajMjehuric(epruvetaIndex, epX, epY);

    for (int j = 0; j < 20; j++) {
        if (vanjskiMjehuri[epruvetaIndex][j].zivot > 0) {
            vanjskiMjehuri[epruvetaIndex][j].x += vanjskiMjehuri[epruvetaIndex][j].brzinaX;
            vanjskiMjehuri[epruvetaIndex][j].y += vanjskiMjehuri[epruvetaIndex][j].brzinaY;
            vanjskiMjehuri[epruvetaIndex][j].brzinaX *= 0.995f;
            vanjskiMjehuri[epruvetaIndex][j].brzinaY -= 0.015f;
            vanjskiMjehuri[epruvetaIndex][j].zivot--;

            int alpha = (int)((vanjskiMjehuri[epruvetaIndex][j].zivot / 140.0f) * 200.0f);
            if (alpha < 0) alpha = 0;
            if (alpha > 255) alpha = 255;

            sf::CircleShape glow(vanjskiMjehuri[epruvetaIndex][j].velicina + 2.f);
            glow.setPosition({
                vanjskiMjehuri[epruvetaIndex][j].x - 2.f,
                vanjskiMjehuri[epruvetaIndex][j].y - 2.f
                });
            glow.setFillColor(sf::Color(50, 255, 100, alpha / 3));
            window.draw(glow);

            sf::CircleShape mjeh(vanjskiMjehuri[epruvetaIndex][j].velicina);
            mjeh.setPosition({
                vanjskiMjehuri[epruvetaIndex][j].x,
                vanjskiMjehuri[epruvetaIndex][j].y
                });
            mjeh.setFillColor(sf::Color(50, 255, 100, alpha));
            window.draw(mjeh);
        }
    }
}

void nacrtajUnutrasnjeMjehurice(float epX, float epY, int nivo, float tajmerKruzica) {
    for (int j = 0; j < 8; j++) {
    
        int seed = (int)(epX * 10) + j * 137 + (int)(tajmerKruzica * 3.0f) % 100;
        int osnovniX = 5 + (seed * 7 + 3) % 18;
        int osnovniY = (nivo == PUNA) ? 20 + (seed * 11 + 5) % 65 : 45 + (seed * 11 + 5) % 35;
        float brzinaGore = 0.3f + ((seed * 3 + 1) % 10) / 10.0f;
        float pomakGore = ((int)(tajmerKruzica * brzinaGore * 10.0f + j * 50) % 70);
        float treperenje = ((seed + (int)(tajmerKruzica * 2.0f)) % 7) - 3;

        float mjehX = osnovniX + treperenje;
        float mjehY = osnovniY - pomakGore;

        float gornjaGranica = 15.0f;
        float donjaGranica = (nivo == PUNA) ? 90.0f : 55.0f;

        // Granice kretanja
        if (mjehY < gornjaGranica) mjehY = donjaGranica - (gornjaGranica - mjehY);
        if (mjehY > donjaGranica) mjehY = gornjaGranica + (mjehY - donjaGranica);
        if (mjehX < 4.0f) mjehX = 4.0f;
        if (mjehX > 24.0f) mjehX = 24.0f;

        float velicina = 2.0f + ((seed * 13 + 7) % 31) / 10.0f;

        sf::CircleShape glow(velicina + 1.5f);
        glow.setPosition({ epX + mjehX - 1.5f, epY + mjehY - 1.5f });
        int alpha = 120 + ((seed * 5 + 2) % 100);
        if (alpha > 220) alpha = 220;
        glow.setFillColor(sf::Color(50 + (seed % 50), 200 + (seed % 55), 100, alpha / 3));
        window.draw(glow);

        sf::CircleShape mjeh(velicina);
        mjeh.setPosition({ epX + mjehX, epY + mjehY });
        mjeh.setFillColor(sf::Color(50 + (seed % 50), 200 + (seed % 55), 100, alpha));
        window.draw(mjeh);
    }
}

void nacrtajStalakIEpruvete(int nivoi[]) {

    nacrtajSjenu(STALAK_X, STALAK_Y + 135.f, 234.f, 10.f, 15.f);

    sf::RectangleShape sipkaL_vanjska({ 10.0f, 145.0f });
    sipkaL_vanjska.setPosition({ STALAK_X - 5.0f, STALAK_Y + 25.0f });
    sipkaL_vanjska.setFillColor(sf::Color(100, 100, 105));
    window.draw(sipkaL_vanjska);

    sf::RectangleShape sipkaL({ 6.0f, 143.0f });
    sipkaL.setPosition({ STALAK_X - 3.0f, STALAK_Y + 26.0f });
    sipkaL.setFillColor(sf::Color(140, 140, 145));
    sipkaL.setOutlineColor(sf::Color(100, 100, 105));
    sipkaL.setOutlineThickness(1.0f);
    window.draw(sipkaL);

    sf::RectangleShape sipkaD_vanjska({ 10.0f, 145.0f });
    sipkaD_vanjska.setPosition({ STALAK_X + 225.0f, STALAK_Y + 25.0f });
    sipkaD_vanjska.setFillColor(sf::Color(100, 100, 105));
    window.draw(sipkaD_vanjska);

    sf::RectangleShape sipkaD({ 6.0f, 143.0f });
    sipkaD.setPosition({ STALAK_X + 227.0f, STALAK_Y + 26.0f });
    sipkaD.setFillColor(sf::Color(140, 140, 145));
    sipkaD.setOutlineColor(sf::Color(100, 100, 105));
    sipkaD.setOutlineThickness(1.0f);
    window.draw(sipkaD);

    sf::RectangleShape odsjajL({ 2.0f, 100.0f });
    odsjajL.setPosition({ STALAK_X - 2.0f, STALAK_Y + 30.0f });
    odsjajL.setFillColor(sf::Color(200, 200, 210, 80));
    window.draw(odsjajL);

    sf::RectangleShape odsjajD({ 2.0f, 100.0f });
    odsjajD.setPosition({ STALAK_X + 228.0f, STALAK_Y + 30.0f });
    odsjajD.setFillColor(sf::Color(200, 200, 210, 80));
    window.draw(odsjajD);

    sf::RectangleShape gornjiDrzac({ 230.0f, 12.0f });
    gornjiDrzac.setPosition({ STALAK_X, STALAK_Y + 35.0f });
    gornjiDrzac.setFillColor(sf::Color(170, 170, 175));
    gornjiDrzac.setOutlineColor(sf::Color(120, 120, 125));
    gornjiDrzac.setOutlineThickness(1.5f);
    window.draw(gornjiDrzac);

    sf::RectangleShape gornjiRub({ 230.0f, 3.0f });
    gornjiRub.setPosition({ STALAK_X, STALAK_Y + 35.0f });
    gornjiRub.setFillColor(sf::Color(200, 200, 205));
    window.draw(gornjiRub);

    sf::RectangleShape donjiDrzac({ 230.0f, 14.0f });
    donjiDrzac.setPosition({ STALAK_X, STALAK_Y + 135.0f });
    donjiDrzac.setFillColor(sf::Color(130, 130, 135));
    donjiDrzac.setOutlineColor(sf::Color(90, 90, 95));
    donjiDrzac.setOutlineThickness(1.5f);
    window.draw(donjiDrzac);

    sf::RectangleShape donjiRub({ 230.0f, 3.0f });
    donjiRub.setPosition({ STALAK_X, STALAK_Y + 135.0f });
    donjiRub.setFillColor(sf::Color(170, 170, 175));
    window.draw(donjiRub);

    // EPRUVETE
    sf::Color boje[4] = {
        TOXIC_ZELENA, TOXIC_ZUTA, TOXIC_NARANDZASTA, TOXIC_TAMNA
    };

    tajmerMjehurica = tajmerMjehurica + 0.05f;
    if (tajmerMjehurica > 10000.0f)  tajmerMjehurica = 0.0f;

    for (int i = 0; i < BROJ_EPRUVETA; i++) {

        float epX = STALAK_X + 20.f + i * 50.f;
        float epY = STALAK_Y;

        nacrtajEpruvetu(epX, epY, nivoi[i], boje[i]);

        if (nivoi[i] == POLUPUNA || nivoi[i] == PUNA) {
            nacrtajUnutrasnjeMjehurice(epX, epY, nivoi[i], tajmerMjehurica);
            azurirajVanjskeMjehurice(epX, epY, i);
        }
    }
}
void nacrtajGlavnuSlikuMolekula() {
    float slikaX = GLAVNA_SLIKA_X;
    float slikaY = GLAVNA_SLIKA_Y;
    float slikaW = GLAVNA_SLIKA_W;
    float slikaH = GLAVNA_SLIKA_H;

    float cx = slikaX + 95.f;
    float cy = slikaY + 70.f;
    float rPrstena = 30.f;

    sf::RectangleShape ram({ slikaW, slikaH });
    ram.setPosition({ slikaX, slikaY });
    ram.setFillColor(SLIKA_POZADINA);
    ram.setOutlineColor(RAM_DARK);
    ram.setOutlineThickness(6.f);
    window.draw(ram);

    sf::CircleShape prsten(rPrstena, 6);
    prsten.setOrigin({ rPrstena, rPrstena });
    prsten.setPosition({ cx, cy });
    prsten.setFillColor(sf::Color::Transparent);
    prsten.setOutlineColor(HEMIJA_LJUBICASTA);
    prsten.setOutlineThickness(4.f);
    prsten.setRotation(sf::degrees(30.f));
    window.draw(prsten);

    sf::CircleShape prstenUnutrasnji(rPrstena - 8.f, 6);
    prstenUnutrasnji.setOrigin({ rPrstena - 8.f, rPrstena - 8.f });
    prstenUnutrasnji.setPosition({ cx, cy });
    prstenUnutrasnji.setFillColor(sf::Color::Transparent);
    prstenUnutrasnji.setOutlineColor(HEMIJA_PLAVA);
    prstenUnutrasnji.setOutlineThickness(2.f);
    prstenUnutrasnji.setRotation(sf::degrees(30.f));
    window.draw(prstenUnutrasnji);

    sf::VertexArray veze(sf::PrimitiveType::Lines, 12);
    veze[0].position = { cx + 15.f, cy - 26.f }; veze[1].position = { cx + 42.f, cy - 52.f };
    veze[2].position = { cx - 15.f, cy - 26.f }; veze[3].position = { cx - 42.f, cy - 52.f };
    veze[4].position = { cx + 30.f, cy };        veze[5].position = { cx + 62.f, cy };
    veze[6].position = { cx - 30.f, cy };        veze[7].position = { cx - 62.f, cy };
    veze[8].position = { cx + 20.f, cy - 18.f }; veze[9].position = { cx + 38.f, cy - 38.f };
    veze[10].position = { cx - 20.f, cy - 18.f }; veze[11].position = { cx - 38.f, cy - 38.f };
    for (int i = 0; i < 12; i++) veze[i].color = HEMIJA_PLAVA;
    window.draw(veze);

    sf::CircleShape ljubicasti(8.f); //ljubicasti atomi
    ljubicasti.setOrigin({ 8.f, 8.f });
    ljubicasti.setFillColor(HEMIJA_LJUBICASTA);
    nacrtajGlow(cx + 42.f, cy - 52.f, 12.f, HEMIJA_LJUBICASTA, 40);
    ljubicasti.setPosition({ cx + 42.f, cy - 52.f });
    window.draw(ljubicasti);
    nacrtajGlow(cx - 42.f, cy - 52.f, 12.f, HEMIJA_LJUBICASTA, 40);
    ljubicasti.setPosition({ cx - 42.f, cy - 52.f });
    window.draw(ljubicasti);

    sf::CircleShape plavi(6.f); //plavi atomi
    plavi.setOrigin({ 6.f, 6.f });
    plavi.setFillColor(HEMIJA_PLAVA);
    plavi.setPosition({ cx + 62.f, cy });
    window.draw(plavi);
    plavi.setPosition({ cx - 62.f, cy });
    window.draw(plavi);
    plavi.setPosition({ cx, cy - 35.f });
    window.draw(plavi);
    plavi.setPosition({ cx + 30.f, cy + 30.f });
    window.draw(plavi);
    plavi.setPosition({ cx - 30.f, cy + 30.f });
    window.draw(plavi);
}
void generisiMaleSlike(Pravougaonik maleSlike[]) {
    Pravougaonik zauzeto[10];
    int brojZauzetih = 0;

    zauzeto[brojZauzetih++] = { GLAVNA_SLIKA_X, GLAVNA_SLIKA_Y, GLAVNA_SLIKA_W, GLAVNA_SLIKA_H };
    zauzeto[brojZauzetih++] = { STALAK_X, STALAK_Y + 10.f, 234.f, 145.f };
    zauzeto[brojZauzetih++] = { DESTILACIJA_X, DESTILACIJA_Y, 300.f, 300.f };
    zauzeto[brojZauzetih++] = { MIKROSKOP_X, MIKROSKOP_Y, 84.f, 120.f };
    zauzeto[brojZauzetih++] = { STO_X,STO_Y, STO_W, STO_H};
    zauzeto[brojZauzetih++] = { STO_X + STO_W - 45.f,STO_Y - 120.f, 40.f,120.f };
    zauzeto[brojZauzetih++] ={ 0.f, UPOZORENJE_Y, (float)W,  12.f };

    for (int i = 0; i < BROJ_MALIH_SLIKA; i++) {
        bool dobraPozicija = false;
        Pravougaonik nova;
        int pokusaja = 0;

        while (!dobraPozicija && pokusaja < 1000) {
            nova.x = 40.f + std::rand() % 611;
            nova.y = 45.f + std::rand() % 206;
            nova.w = 85.f;
            nova.h = 60.f;

            dobraPozicija = true;

            for (int j = 0; j < brojZauzetih; j++) {
                if (preklapajuSe(nova, zauzeto[j])) {
                    dobraPozicija = false;
                    break;
                }
            }

            pokusaja++;
        }

        if (dobraPozicija) {
            maleSlike[i] = nova;
            zauzeto[brojZauzetih++] = nova;
        }
        else {
            maleSlike[i] = { 50.f + i * 200.f, 50.f, 85.f, 60.f };
        }
    }
}
void nacrtajMaluSliku(Pravougaonik r, int tip) {

    sf::RectangleShape ram({ r.w, r.h });
    ram.setPosition({ r.x, r.y });
    ram.setFillColor(SLIKA_POZADINA);
    ram.setOutlineColor(RAM_DARK);
    ram.setOutlineThickness(4.f);
    window.draw(ram);

    if (tip == 0) {
        // H2O
        // H
        sf::CircleShape h1(8.f);
        h1.setPosition({ r.x + 15.f, r.y + 20.f });
        h1.setFillColor(HEMIJA_LJUBICASTA);
        window.draw(h1);
        // O
        sf::CircleShape o(12.f);
        o.setPosition({ r.x + 32.f, r.y + 16.f });
        o.setFillColor(HEMIJA_PLAVA);
        window.draw(o);
        // H
        sf::CircleShape h2(8.f);
        h2.setPosition({ r.x + 55.f, r.y + 20.f });
        h2.setFillColor(HEMIJA_LJUBICASTA);
        window.draw(h2);

        sf::VertexArray veze(sf::PrimitiveType::Lines, 4);
        veze[0].position = { r.x + 15.f+8.f, r.y + 20.f+8.f }; veze[1].position = { r.x + 32.f+12.f, r.y + 16.f+12.f };
        veze[2].position = { r.x + 44.f, r.y + 28.f }; veze[3].position = { r.x + 55.f, r.y + 28.f };
        for (int i = 0; i < 4; i++) veze[i].color = HEMIJA_PLAVA;
        window.draw(veze);
    }
    else if (tip == 1) {
     
        sf::ConvexShape flask;
        flask.setPointCount(4);
        flask.setPoint(0, { r.x + 30.f, r.y + 15.f });
        flask.setPoint(1, { r.x + 55.f, r.y + 15.f });
        flask.setPoint(2, { r.x + 65.f, r.y + 50.f });
        flask.setPoint(3, { r.x + 20.f, r.y + 50.f });
        flask.setFillColor(HEMIJA_ZELENA);
        flask.setOutlineColor(HEMIJA_LJUBICASTA);
        flask.setOutlineThickness(2.f);
        window.draw(flask);

        sf::ConvexShape tecnost;
        tecnost.setPointCount(4);
        tecnost.setPoint(0, { r.x + 32.f, r.y + 30.f });
        tecnost.setPoint(1, { r.x + 53.f, r.y + 30.f });
        tecnost.setPoint(2, { r.x + 60.f, r.y + 48.f });
        tecnost.setPoint(3, { r.x + 25.f, r.y + 48.f });
        tecnost.setFillColor(HEMIJA_NARANDZASTA);
        window.draw(tecnost);

        sf::CircleShape m1(3.f); m1.setPosition({ r.x + 35.f, r.y + 35.f }); m1.setFillColor(TOXIC_SVIJETLA); window.draw(m1);
        sf::CircleShape m2(2.f); m2.setPosition({ r.x + 45.f, r.y + 38.f }); m2.setFillColor(TOXIC_SVIJETLA); window.draw(m2);
    }
    else {
        float cx = r.x + r.w / 2.f, cy = r.y + r.h / 2.f;
        for (int i = 0; i < 3; i++) {
            sf::CircleShape orbita(16.f);
            orbita.setOrigin({ 16.f, 16.f });
            orbita.setPosition({ cx, cy });
            orbita.setScale({ 1.8f, 0.6f });
            orbita.setRotation(sf::degrees(i * 60.f));
            orbita.setFillColor(sf::Color::Transparent);
            orbita.setOutlineColor(sf::Color(20, 190, 200));
            orbita.setOutlineThickness(2.f);
            window.draw(orbita);
        }
        sf::CircleShape jezgro(7.f);
        jezgro.setOrigin({ 7.f, 7.f });
        jezgro.setPosition({ cx, cy });
        jezgro.setFillColor(sf::Color(255, 90, 95));
        window.draw(jezgro);

        sf::CircleShape e1(4.f); e1.setOrigin({ 4.f, 4.f }); e1.setPosition({ cx + 28.f, cy }); e1.setFillColor(sf::Color::Green); window.draw(e1);
        sf::CircleShape e2(4.f); e2.setOrigin({ 4.f, 4.f }); e2.setPosition({ cx - 28.f, cy }); e2.setFillColor(sf::Color::Blue); window.draw(e2);
        sf::CircleShape e3(4.f); e3.setOrigin({ 4.f, 4.f }); e3.setPosition({ cx, cy - 17.f }); e3.setFillColor(sf::Color(255, 180, 0)); window.draw(e3);
        sf::CircleShape e4(4.f); e4.setOrigin({ 4.f, 4.f }); e4.setPosition({ cx, cy + 17.f }); e4.setFillColor(sf::Color(120, 120, 255)); window.draw(e4);
    }
}
void nacrtajMis(float x, float y, int smjer) {
    const float MIS_SIRINA = 100.f;

    float sjenaX = (smjer == 1) ? x + 8.f : x + MIS_SIRINA - 8.f - (20.f * 2.f * 1.8f);
    sf::CircleShape sjena(20.f);
    sjena.setScale({ 1.8f, 0.4f });
    sjena.setPosition({ sjenaX, y + 32.f });
    sjena.setFillColor(sf::Color(0, 0, 0, 30));
    window.draw(sjena);
  
    float tijeloX = (smjer == 1) ? x + 5.f : x + MIS_SIRINA - 5.f - (18.f * 2.f * 1.5f);
    sf::CircleShape tijelo(18.f);
    tijelo.setScale({ 1.5f, 0.9f });
    tijelo.setPosition({ tijeloX, y + 8.f });
    tijelo.setFillColor(sf::Color(160, 160, 160));
    window.draw(tijelo);

    float glavaX = (smjer == 1) ? x + 58.f : x + MIS_SIRINA - 58.f - (2.f * 10.f);
    sf::CircleShape glava(10.f);
    glava.setPosition({ glavaX, y + 10.f });
    glava.setFillColor(sf::Color(155, 155, 155));
    window.draw(glava);

    float okoX = (smjer == 1) ? x + 66.f : x + MIS_SIRINA - 66.f - (2.f * 3.f);
    sf::CircleShape oko(3.f);
    oko.setPosition({ okoX, y + 15.f });
    oko.setFillColor(sf::Color::Black);
    window.draw(oko);

    float uhoVX = (smjer == 1) ? x + 56.f : x + MIS_SIRINA - 56.f - (2.f * 6.f);
    float uhoUX = (smjer == 1) ? x + 58.f : x + MIS_SIRINA - 58.f - (2.f * 4.f);
    sf::CircleShape uhoV(6.f), uhoU(4.f);
    uhoV.setPosition({ uhoVX, y + 2.f });
    uhoU.setPosition({ uhoUX, y + 4.f });
    uhoV.setFillColor(sf::Color(150, 150, 150));
    uhoU.setFillColor(sf::Color(255, 170, 185));
    window.draw(uhoV);
    window.draw(uhoU);
    //sape
    float prednjaX = (smjer == 1) ? x + 50.f : x + MIS_SIRINA - 50.f - 8.f;
    float zadnjaX = (smjer == 1) ? x + 30.f : x + MIS_SIRINA - 30.f - 10.f;
    sf::RectangleShape prednja({ 8.f, 2.2f });
    sf::RectangleShape zadnja({ 10.f, 2.2f });
    prednja.setPosition({ prednjaX, y + 34.f });
    zadnja.setPosition({ zadnjaX, y + 36.f });
    prednja.setFillColor(sf::Color(255, 180, 190));
    zadnja.setFillColor(sf::Color(255, 180, 190));
    window.draw(prednja);
    window.draw(zadnja);

    sf::VertexArray rep(sf::PrimitiveType::Lines, 2);
    if (smjer == 1) {
        rep[0].position = { x + 8.f,  y + 30.f };
        rep[1].position = { x - 24.f, y + 36.f };
    }
    else {
        rep[0].position = { x + MIS_SIRINA - 8.f,  y + 30.f };
        rep[1].position = { x + MIS_SIRINA + 24.f, y + 36.f };
    }
    rep[0].color = sf::Color(230, 150, 160);
    rep[1].color = sf::Color(230, 150, 160);
    window.draw(rep);
}
void nacrtajMikroskop(float x, float y) {
    nacrtajSjenu( x + 5.f, y + 2.f, 60.f, 20.f, 15.f, sf::Color(0, 0, 0, 25));

    sf::RectangleShape baza({ 80.f, 15.f });
    baza.setPosition({ x, y }); 
    baza.setFillColor(sf::Color(60, 70, 60));
    baza.setOutlineColor(sf::Color(180, 140, 20));
    baza.setOutlineThickness(1.5f);
    window.draw(baza);

    sf::RectangleShape stubVanjskiV({ 20.f+4.f, 100.f+4.f });
    stubVanjskiV.setPosition({ (x + 10.f) - (24.f - 20.f) / 2.f, (y - 100.f) - (104.f - 100.f) / 2.f });
    stubVanjskiV.setFillColor(sf::Color(100, 160, 200));
    window.draw(stubVanjskiV);

    sf::RectangleShape stubVertikalni({ 20.f, 100.f });
    stubVertikalni.setPosition({ x + 10.f, y - 100.f });
    stubVertikalni.setFillColor(sf::Color(80, 100, 120));
    window.draw(stubVertikalni);

    sf::RectangleShape stubSjaj({ 6.f, 80.f });
    stubSjaj.setPosition({ (x + 10.f) + (20.f / 2.f) - (6.f / 2.f), y - 90.f });
    stubSjaj.setFillColor(sf::Color(200, 235, 255, 100));
    window.draw(stubSjaj);

    sf::RectangleShape okularVanjski({ 20.f+4.f, 60.f+4.f });
    okularVanjski.setPosition({ (x + 10.f + 20.f + 5.f) - (24.f - 20.f) / 2.f, (y - 100.f - 10.f) - (64.f - 60.f) / 2.f });
    okularVanjski.setFillColor(sf::Color(180, 40, 60));
    window.draw(okularVanjski);

    sf::RectangleShape okular({ 20.f, 60.f });
    okular.setPosition({ x + 10.f + 20.f + 5.f,  y - 100.f - 10.f });
    okular.setFillColor(sf::Color(255, 100, 150));
    okular.setOutlineColor(sf::Color(200, 50, 70));
    okular.setOutlineThickness(1.f);
    window.draw(okular);

    sf::RectangleShape sjajOkular({ 6.f, 50.f });
    sjajOkular.setPosition({ (x + 35.f) + (20.f / 2.f) - (6.f / 2.f), y - 105.f });
    sjajOkular.setFillColor(sf::Color(255, 255, 255, 50));
    window.draw(sjajOkular);

    float okularCentarX = (x + 35.f) + (20.f / 2.f);
    sf::RectangleShape stolica({ 40.f, 10.f });
    stolica.setPosition({ okularCentarX - (40.f / 2.f), (y - 100.f) + (100.f / 2.f) });
    stolica.setFillColor(sf::Color(250, 200, 50));
    stolica.setOutlineColor(sf::Color(200, 160, 40));
    stolica.setOutlineThickness(1.f);
    window.draw(stolica);

    sf::CircleShape staklo(8.f);
    staklo.setPosition({ okularCentarX - 8.f,((y - 100.f) + (100.f / 2.f)) - (8.f / 2.f) }); 
    staklo.setFillColor(sf::Color(200, 230, 255, 120));
    staklo.setOutlineColor(sf::Color(150, 180, 210));
    staklo.setOutlineThickness(1.f);
    window.draw(staklo);

    sf::CircleShape tocak(12.f);
    tocak.setPosition({ x + 8.f, y - 80.f }); 
    tocak.setFillColor(sf::Color(250, 200, 50));
    tocak.setOutlineColor(sf::Color(200, 160, 40));
    tocak.setOutlineThickness(1.5f);
    window.draw(tocak);

    sf::CircleShape tocakUnutrasnji(8.f);
    tocakUnutrasnji.setPosition({ ((x + 8.f) + 12.f - 8.f), ((y - 80.f) + 12.f - 8.f) });
    tocakUnutrasnji.setFillColor(sf::Color(220, 180, 40));
    window.draw(tocakUnutrasnji);
}

void nacrtajTronozac(float x, float y) {
    float tronožacY = y + 195.f;

    sf::RectangleShape nogaL({ 5.f, 80.f });
    nogaL.setPosition({ x + 35.f, tronožacY });
    nogaL.setRotation(sf::degrees(12.f));
    nogaL.setFillColor(sf::Color(80, 80, 80));
    nogaL.setOutlineColor(sf::Color(60, 60, 60));
    nogaL.setOutlineThickness(1.f);
    window.draw(nogaL);

    sf::RectangleShape sjajL({ 2.f, 60.f });
    sjajL.setPosition({ x + 36.f, tronožacY + 5.f });
    sjajL.setRotation(sf::degrees(12.f));
    sjajL.setFillColor(sf::Color(140, 140, 140, 120));
    window.draw(sjajL);

    sf::RectangleShape nogaD({ 5.f, 80.f });
    nogaD.setPosition({ x + 125.f, tronožacY });
    nogaD.setRotation(sf::degrees(-12.f));
    nogaD.setFillColor(sf::Color(80, 80, 80));
    nogaD.setOutlineColor(sf::Color(60, 60, 60));
    nogaD.setOutlineThickness(1.f);
    window.draw(nogaD);

    sf::RectangleShape sjajD({ 2.f, 60.f });
    sjajD.setPosition({ x + 126.f, tronožacY + 5.f });
    sjajD.setRotation(sf::degrees(-12.f));
    sjajD.setFillColor(sf::Color(140, 140, 140, 120));
    window.draw(sjajD);

    sf::RectangleShape mreza({ 90.f, 6.f });
    mreza.setPosition({ x + 38.f, tronožacY });
    mreza.setFillColor(sf::Color(120, 120, 120));
    mreza.setOutlineColor(sf::Color(100, 100, 100));
    mreza.setOutlineThickness(1.f);
    window.draw(mreza);

    sf::RectangleShape mrezaRub({ 84.f, 2.f });
    mrezaRub.setPosition({ x + 38.f, tronožacY });
    mrezaRub.setFillColor(sf::Color(160, 160, 160));
    window.draw(mrezaRub);
}
void nacrtajLijeviStativ(float x, float y, float hvataljkaY) {
  
    sf::RectangleShape sipka({ 8.f, 270.f });
    sipka.setPosition({ x + 20.f, y + 10.f });
    sipka.setFillColor(sf::Color(45, 45, 45));
    sipka.setOutlineColor(sf::Color(30, 30, 30));
    sipka.setOutlineThickness(1.f);
    window.draw(sipka);
    nacrtajSjenu(x - 11.f, y + 270.f, 70.f, 10.f, 8.f);

    sf::RectangleShape baza({ 70.f, 10.f });
    baza.setPosition({ x - 11.f, y + 270.f });
    baza.setFillColor(sf::Color(55, 55, 55));
    baza.setOutlineColor(sf::Color(40, 40, 40));
    baza.setOutlineThickness(1.f);
    window.draw(baza);

    sf::RectangleShape hvataljka({ 50.f, 10.f });
    hvataljka.setPosition({ x + 20.f, hvataljkaY });
    hvataljka.setFillColor(sf::Color(80, 80, 80));
    hvataljka.setOutlineColor(sf::Color(60, 60, 60));
    hvataljka.setOutlineThickness(1.f);
    window.draw(hvataljka);

    sf::RectangleShape prsten({ 20.f, 16.f });
    prsten.setPosition({ x + 5.f, hvataljkaY - 3.f });
    prsten.setFillColor(sf::Color(100, 100, 100));
    prsten.setOutlineColor(sf::Color(80, 80, 80));
    prsten.setOutlineThickness(1.f);
    window.draw(prsten);
}

void nacrtajDesniStativ(float x, float y, float hvataljkaY) {
   
    sf::RectangleShape sipka({ 8.f, 270.f });
    sipka.setPosition({ x + 20.f, y + 10.f });
    sipka.setFillColor(sf::Color(45, 45, 45));
    sipka.setOutlineColor(sf::Color(30, 30, 30));
    sipka.setOutlineThickness(1.f);
    window.draw(sipka);
    nacrtajSjenu(x - 11.f, y + 270.f, 70.f, 10.f, 8.f);

    sf::RectangleShape baza({ 70.f, 10.f });
    baza.setPosition({ x - 11.f, y + 270.f });
    baza.setFillColor(sf::Color(55, 55, 55));
    baza.setOutlineColor(sf::Color(40, 40, 40));
    baza.setOutlineThickness(1.f);
    window.draw(baza);

    sf::RectangleShape hvataljka({ 50.f, 10.f });
    hvataljka.setPosition({ x + 20.f, hvataljkaY });
    hvataljka.setFillColor(sf::Color(80, 80, 80));
    hvataljka.setOutlineColor(sf::Color(60, 60, 60));
    hvataljka.setOutlineThickness(1.f);
    window.draw(hvataljka);
}
void nacrtajGorionik(float x, float y) {
    sf::CircleShape baza(20.f);
    baza.setPosition({ x + 60.f, y + 235.f });
    baza.setFillColor(sf::Color(200, 220, 240, 200));
    baza.setOutlineColor(sf::Color(120, 160, 200));
    baza.setOutlineThickness(2.f);
    window.draw(baza);

    sf::CircleShape bazaSjaj(8.f);
    bazaSjaj.setPosition({ x + 68.f, y + 242.f });
    bazaSjaj.setFillColor(sf::Color(240, 250, 255, 150));
    window.draw(bazaSjaj);

    sf::RectangleShape vrat({ 12.f, 14.f });
    vrat.setPosition({ x + 74.f, y + 223.f });
    vrat.setFillColor(sf::Color(170, 170, 170));
    vrat.setOutlineColor(sf::Color(140, 140, 140));
    vrat.setOutlineThickness(1.f);
    window.draw(vrat);

    sf::CircleShape tocakPlavi(4.f);
    tocakPlavi.setPosition({ x + 78.f, y + 228.f });
    tocakPlavi.setFillColor(sf::Color(60, 120, 220));
    window.draw(tocakPlavi);

    sf::RectangleShape cijev({ 8.f, 25.f });
    cijev.setPosition({ x + 76.f, y + 198.f });
    cijev.setFillColor(sf::Color(180, 180, 180));
    cijev.setOutlineColor(sf::Color(140, 140, 140));
    cijev.setOutlineThickness(1.f);
    window.draw(cijev);
}

void nacrtajPlamen(float x, float y) {
    float vrhY = y + 195.f + (std::rand() % 9) - 4;
    float treperenjeX = (std::rand() % 4) - 1.5f;

    nacrtajGlow(x + 80.f + treperenjeX, vrhY + 15.f, 15.f, sf::Color(255, 90, 0), 40);

    sf::ConvexShape vanjski;
    vanjski.setPointCount(3);
    vanjski.setPoint(0, { x + 80.f + treperenjeX, vrhY });
    vanjski.setPoint(1, { x + 67.f + (std::rand() % 3) - 1, y + 225.f });
    vanjski.setPoint(2, { x + 93.f + (std::rand() % 3) - 1, y + 225.f });
    vanjski.setFillColor(sf::Color(255, 90, 0, 230));
    window.draw(vanjski);

    sf::ConvexShape srednji;
    srednji.setPointCount(3);
    srednji.setPoint(0, { x + 80.f + treperenjeX * 0.7f, vrhY + 8.f });
    srednji.setPoint(1, { x + 72.f, y + 225.f });
    srednji.setPoint(2, { x + 88.f, y + 225.f });
    srednji.setFillColor(sf::Color(255, 220, 50, 240));
    window.draw(srednji);

    sf::ConvexShape jezgro;
    jezgro.setPointCount(3);
    jezgro.setPoint(0, { x + 80.f + treperenjeX * 0.3f, y + 210.f + (std::rand() % 5) - 2 });
    jezgro.setPoint(1, { x + 75.f, y + 225.f });
    jezgro.setPoint(2, { x + 85.f, y + 225.f });
    jezgro.setFillColor(sf::Color(100, 210, 255, 255));
    window.draw(jezgro);
}

void nacrtajGlavnuBocu(float x, float y) {
    float bocaX = x + 40.f;
    float bocaY = y + 85.f;

    nacrtajGlow(bocaX + 40.f, bocaY + 40.f, 50.f, sf::Color(255, 180, 30), 40);

    sf::CircleShape staklo(40.f);
    staklo.setPosition({ bocaX, bocaY });
    staklo.setFillColor(sf::Color(230, 245, 255, 80));
    staklo.setOutlineColor(sf::Color(180, 210, 235));
    staklo.setOutlineThickness(2.5f);
    window.draw(staklo);

    sf::CircleShape sjaj(18.f);
    sjaj.setPosition({ bocaX + 10.f, bocaY + 12.f });
    sjaj.setFillColor(sf::Color(255, 255, 255, 50));
    sjaj.setScale({ 1.6f, 1.0f });
    window.draw(sjaj);

    sf::ConvexShape tecnost;
    tecnost.setPointCount(6);
    tecnost.setPoint(0, { bocaX + 5.f, bocaY + 50.f });
    tecnost.setPoint(1, { bocaX + 75.f, bocaY + 50.f });
    tecnost.setPoint(2, { bocaX + 70.f, bocaY + 65.f });
    tecnost.setPoint(3, { bocaX + 55.f, bocaY + 78.f });
    tecnost.setPoint(4, { bocaX + 25.f, bocaY + 78.f });
    tecnost.setPoint(5, { bocaX + 10.f, bocaY + 65.f });
    tecnost.setFillColor(TOXIC_NARANDZASTA);
    window.draw(tecnost);

    for (int i = 0; i < 5; i++) {
        float mx = (std::rand() % 41) + 20;
        float my = (std::rand() % 21) + 52;
        float vel = (std::rand() % 3) + 2;
        sf::CircleShape mjeh(vel);
        mjeh.setPosition({ bocaX + mx, bocaY + my });
        mjeh.setFillColor(TOXIC_ZUTA);
        window.draw(mjeh);
    }

    sf::RectangleShape vrat({ 18.f, 45.f });
    vrat.setPosition({ bocaX + 31.f, bocaY - 35.f });
    vrat.setFillColor(sf::Color(230, 245, 255, 120));
    vrat.setOutlineColor(sf::Color(180, 210, 235));
    vrat.setOutlineThickness(2.f);
    window.draw(vrat);

    sf::RectangleShape cep({ 22.f, 15.f });
    cep.setPosition({ bocaX + 29.f, bocaY - 45.f });
    cep.setFillColor(sf::Color(210, 140, 80));
    cep.setOutlineColor(sf::Color(170, 110, 50));
    cep.setOutlineThickness(1.f);
    window.draw(cep);

    sf::RectangleShape obujmica({ 26.f, 4.f });
    obujmica.setPosition({ bocaX + 27.f, bocaY - 25.f });
    obujmica.setFillColor(sf::Color(120, 120, 120));
    window.draw(obujmica);
}

void nacrtajKondenzator(float x, float y) {
    int brojKrugova = 8;

    for (int i = 0; i < brojKrugova; i++) {
        float ky = y + (i * 16.f);

        sf::CircleShape prsten(22.f);
        prsten.setScale({ 1.0f, 0.45f });
        prsten.setPosition({ x, ky });
        prsten.setFillColor(sf::Color(210, 240, 255, 40));
        prsten.setOutlineColor(sf::Color(135, 200, 235, 180));
        prsten.setOutlineThickness(3.f);
        window.draw(prsten);

        sf::CircleShape unutrasnji(22.f-3.f);
        unutrasnji.setScale({ 1.0f, 0.45f });
        unutrasnji.setPosition({ x + 3.f, ky + 3.f });
        unutrasnji.setFillColor(sf::Color(255, 255, 255, 18));
        window.draw(unutrasnji);
    }
}
void nacrtajDizalniStalak(float x, float y) {
 
    nacrtajSjenu(x - 5.f, y + 4.f, 110.f, 15.f, 10.f);

    sf::RectangleShape gornjaPloča({ 100.f, 8.f });
    gornjaPloča.setPosition({ x, y });
    gornjaPloča.setFillColor(sf::Color(160, 160, 165));
    gornjaPloča.setOutlineColor(sf::Color(120, 120, 125));
    gornjaPloča.setOutlineThickness(1.f);
    window.draw(gornjaPloča);

    sf::RectangleShape gornjiRub({ 100.f, 2.f });
    gornjiRub.setPosition({ x, y });
    gornjiRub.setFillColor(sf::Color(200, 200, 205));
    window.draw(gornjiRub);

    sf::RectangleShape donjaPloča({ 100.f, 8.f });
    donjaPloča.setPosition({ x, y + 42.f });
    donjaPloča.setFillColor(sf::Color(140, 140, 145));
    donjaPloča.setOutlineColor(sf::Color(100, 100, 105));
    donjaPloča.setOutlineThickness(1.f);
    window.draw(donjaPloča);

    sf::RectangleShape donjiRub({ 100.f, 2.f });
    donjiRub.setPosition({ x, y + 42.f });
    donjiRub.setFillColor(sf::Color(180, 180, 185));
    window.draw(donjiRub);

    sf::RectangleShape nogaL1({ 4.f, 35.f });
    nogaL1.setPosition({ x + 25.f, y + 8.f });
    nogaL1.setRotation(sf::degrees(-25.f));
    nogaL1.setFillColor(sf::Color(100, 100, 105));
    window.draw(nogaL1);

    sf::RectangleShape nogaL2({ 4.f, 35.f });
    nogaL2.setPosition({ x + 25.f, y + 8.f });
    nogaL2.setRotation(sf::degrees(25.f));
    nogaL2.setFillColor(sf::Color(100, 100, 105));
    window.draw(nogaL2);

    sf::RectangleShape nogaD1({ 4.f, 35.f });
    nogaD1.setPosition({ x + 75.f, y + 8.f });
    nogaD1.setRotation(sf::degrees(25.f));
    nogaD1.setFillColor(sf::Color(100, 100, 105));
    window.draw(nogaD1);

    sf::RectangleShape nogaD2({ 4.f, 35.f });
    nogaD2.setPosition({ x + 75.f, y + 8.f });
    nogaD2.setRotation(sf::degrees(-25.f));
    nogaD2.setFillColor(sf::Color(100, 100, 105));
    window.draw(nogaD2);

    sf::RectangleShape vijak({ 50.f, 4.f });
    vijak.setPosition({ x + 25.f, y + 23.f });
    vijak.setFillColor(sf::Color(80, 80, 85));
    window.draw(vijak);

    sf::CircleShape rucka(5.f);
    rucka.setPosition({ x + 20.f, y + 20.f });
    rucka.setFillColor(sf::Color(120, 120, 125));
    window.draw(rucka);
}

void nacrtajCijevi(float bocaVratX, float bocaVratY, float kondX, float kondY) {
  
    sf::RectangleShape cijev1({ 6.f, 53.f });
    cijev1.setPosition({ bocaVratX - 3.f, bocaVratY - 50.f });
    cijev1.setFillColor(sf::Color(135, 205, 240, 220));
    cijev1.setOutlineColor(sf::Color(100, 170, 210));
    cijev1.setOutlineThickness(1.f);
    window.draw(cijev1);

    // Horizontalna cijev od boce do kondenzatora
    float hStart = bocaVratX - 3.f;
    float hEnd = kondX + 20.f; 
    float hWidth = hEnd - hStart;
    sf::RectangleShape cijev2({ hWidth, 6.f });
    cijev2.setPosition({ hStart, bocaVratY - 50.f });
    cijev2.setFillColor(sf::Color(135, 205, 240, 220));
    cijev2.setOutlineColor(sf::Color(100, 170, 210));
    cijev2.setOutlineThickness(1.f);
    window.draw(cijev2);

    // Vertikalna cijev dolje u kondenzator
    sf::RectangleShape cijev3({ 6.f, 20.f });
    cijev3.setPosition({ kondX + 20.f - 3.f, bocaVratY - 50.f });
    cijev3.setFillColor(sf::Color(135, 205, 240, 220));
    cijev3.setOutlineColor(sf::Color(100, 170, 210));
    cijev3.setOutlineThickness(1.f);
    window.draw(cijev3);

}
void nacrtajErlenmajer(float erlenX, float erlenY)
{
    sf::ConvexShape staklo;
    staklo.setPointCount(4);
    staklo.setPoint(0, { erlenX + 20.f, erlenY });
    staklo.setPoint(1, { erlenX + 40.f, erlenY });
    staklo.setPoint(2, { erlenX + 65.f, erlenY + 85.f });
    staklo.setPoint(3, { erlenX - 5.f, erlenY + 85.f });
    staklo.setFillColor(sf::Color(220, 245, 255, 90));
    staklo.setOutlineColor(sf::Color(160, 200, 235));
    staklo.setOutlineThickness(2.5f);
    window.draw(staklo);

    sf::CircleShape sjaj(14.f);
    sjaj.setPosition({ erlenX + 12.f, erlenY + 17.f });
    sjaj.setFillColor(sf::Color(255, 255, 255, 45));
    sjaj.setScale({ 1.2f, 0.9f });
    window.draw(sjaj);

    sf::RectangleShape cep({ 24.f, 12.f });
    cep.setPosition({ erlenX + 18.f, erlenY - 5.f });
    cep.setFillColor(sf::Color(210, 140, 80));
    cep.setOutlineColor(sf::Color(170, 110, 50));
    cep.setOutlineThickness(1.f);
    window.draw(cep);

    sf::RectangleShape adapter({ 14.f, 16.f });
    adapter.setPosition({ erlenX + 23.f, erlenY - 5.f });
    adapter.setFillColor(sf::Color(100, 100, 100));
    adapter.setOutlineColor(sf::Color(80, 80, 80));
    adapter.setOutlineThickness(1.f);
    window.draw(adapter);

    sf::ConvexShape tecnost;
    tecnost.setPointCount(4);
    tecnost.setPoint(0, { erlenX + 5.f + 5.f, erlenY + 35.f + 5.f });      
    tecnost.setPoint(1, { erlenX + 45.f + 5.f, erlenY + 35.f + 5.f });    
    tecnost.setPoint(2, { erlenX + 58.f + 5.f, erlenY + 78.f + 5.f });    
    tecnost.setPoint(3, { erlenX - 8.f + 5.f, erlenY + 78.f + 5.f });     
    tecnost.setFillColor(TOXIC_ZELENA);
    window.draw(tecnost);

    sf::CircleShape meniskus(19.f);
    meniskus.setScale({ 1.4f, 0.4f });
    meniskus.setPosition({ erlenX + 2.f, erlenY + 32.f + 5.f });
    meniskus.setFillColor(sf::Color(255, 255, 255, 30));
    window.draw(meniskus);

    // Mjehurići
    for (int i = 0; i < 5; i++) {
        float mx = 8.f + (std::rand() % 35);
        float my = 40.f + (std::rand() % 30);
        float vel = 2.f + (std::rand() % 4);
        sf::CircleShape mjeh(vel);
        mjeh.setPosition({ erlenX + mx + 5.f, erlenY + my + 5.f });
        mjeh.setFillColor(TOXIC_SVIJETLA);
        window.draw(mjeh);
    }

}
void nacrtajKapljice(float izlazX, float izlazY, float erlenVrhY, float& brzinaY) {

    const float GRAVITACIJA = 0.4f;
    brzinaY += GRAVITACIJA;

    float pad = (erlenVrhY + 37.f) - izlazY;

    if (brzinaY > pad) {
        brzinaY = 0.f;
    }

    float startX = izlazX - 3.f;  
    float startY = izlazY;          

    sf::CircleShape kap(3.f);
    kap.setFillColor(TOXIC_ZELENA);
    kap.setPosition({ startX, startY });
    kap.move({ 0.f, brzinaY });
    window.draw(kap);  

    sf::CircleShape kap2(3.f);
    kap2.setFillColor(TOXIC_ZELENA);
    kap2.setPosition({ startX, startY });
    float dy2 = brzinaY - pad / 3.f;
    if (dy2 < 0.f) dy2 += pad;
    kap2.move({ 0.f, dy2 });
    window.draw(kap2);

    sf::CircleShape kap3(2.5f);
    kap3.setFillColor(TOXIC_SVIJETLA);
    kap3.setPosition({ startX + 1.f, startY });
    float dy3 = brzinaY - 2.f * pad / 3.f;
    if (dy3 < 0.f) dy3 += pad;
    kap3.move({ 0.f, dy3 });
    window.draw(kap3);
}
void nacrtajIzlazneCijevi(float kondX, float kondY, float& izlazX, float& izlazY) {

    // 1. Horizontalna cijev iz desnog dijela  kondenzatora
    float hStartX = kondX + (2.f*22.f);     
    float hStartY = kondY + (5.f * 16.f) + 10.f;       

    sf::RectangleShape h({ 45.f, 6.f });
    h.setPosition({ hStartX, hStartY });
    h.setFillColor(sf::Color(135, 205, 240, 220));
    h.setOutlineColor(sf::Color(100, 170, 210));
    h.setOutlineThickness(1.f);
    window.draw(h);

    // 2. Vertikalna cijev 
    float vStartX = hStartX + 45.f - (6.f/2.f); 
    float vStartY = hStartY;              
    const float V_HEIGHT = 40.f;           

    sf::RectangleShape v({ 6.f, V_HEIGHT });
    v.setPosition({ vStartX, vStartY });
    v.setFillColor(sf::Color(135, 205, 240, 220));
    v.setOutlineColor(sf::Color(100, 170, 210));
    v.setOutlineThickness(1.f);
    window.draw(v);

    izlazX = vStartX + (6.f/2.f);     
    izlazY = vStartY + V_HEIGHT;    
}

void nacrtajDestilaciju(float x, float y)
{
    nacrtajLijeviStativ(x, y, y + 55.f);
    nacrtajDesniStativ(x + 145.f, y, y + 65.f);

    nacrtajTronozac(x, y);
    nacrtajGorionik(x, y);
    nacrtajPlamen(x, y);

    nacrtajGlavnuBocu(x, y + 15.f);

    float kondX = x + 150.f;
    float kondY = y + 20.f;
    nacrtajKondenzator(kondX, kondY);

    float bocaVratX = x + 80.f;
    float bocaVratY = y + 50.f;
    nacrtajCijevi(bocaVratX, bocaVratY, kondX, kondY);
 
    float izlazX, izlazY;
    nacrtajIzlazneCijevi(kondX, kondY, izlazX, izlazY);

    float erlenX = izlazX - ((20.f + 40.f) / 2.f);      
    float erlenY = izlazY + 5.f;         

    float stalakX = erlenX - ((100.f - 70.f) / 2.f);     
    float stalakY = erlenY + 85.f;  

    nacrtajDizalniStalak(stalakX, stalakY);
    nacrtajSjenu(stalakX, stalakY + 4.f, 100.f, 10.f, 10.f);
    nacrtajErlenmajer(erlenX, erlenY);
    nacrtajKapljice(izlazX, izlazY, erlenY, brzinaKapljice);
}
void nacrtajMenzuru(float x, float y) {
    float sirina = 40.f;
    float visina = 120.f;
  
    sf::RectangleShape menzura({ sirina, visina });
    menzura.setPosition({ x, y });
    menzura.setFillColor(sf::Color(220, 240, 255, 35));
    menzura.setOutlineColor(sf::Color(200, 220, 255));
    menzura.setOutlineThickness(2.f);
    window.draw(menzura);

    sf::RectangleShape sjaj({ 4.f, visina - 10.f });
    sjaj.setPosition({ x + 5.f, y + 5.f });
    sjaj.setFillColor(sf::Color(255, 255, 255, 60));
    window.draw(sjaj);

    for (int i = 0; i < 10; i++) {
        sf::RectangleShape crta({ 12.f, 2.f });
        crta.setPosition({ x + sirina - 12.f, y + visina - 10.f - i * 10.f });
        crta.setFillColor(sf::Color::White);
        window.draw(crta);
    }
}
int main() {
    std::srand(0);
    inicijalizujMjehurice();

    int nivoi[BROJ_EPRUVETA];

    std::cout << "Unesite nivo za svaku epruvetu:" << std::endl;
    std::cout << "0 - prazna, 1 - polupuna, 2 - puna" << std::endl;

    for (int i = 0; i < BROJ_EPRUVETA; i++) {
        int unos;
        bool validanUnos = false;

        do {
            std::cout << "Epruveta " << i + 1 << ": ";

            if (!(std::cin >> unos)) {
                std::cout << "Greska! Morate unijeti broj." << std::endl;
                std::cin.clear(); 
                std::cin.ignore(10000, '\n');
            }
            else if (unos < 0 || unos > 2) {
                std::cout << "Greska! Unesite 0, 1 ili 2." << std::endl;
            }
            else {
                validanUnos = true;
            }
        } while (!validanUnos);
        nivoi[i] = unos;
    }
    Pravougaonik maleSlike[BROJ_MALIH_SLIKA];
    generisiMaleSlike(maleSlike);

    float misX = STO_X;
    float misY = MIS_Y;

    float brzina = 0.f;
    float ubrzanje = 0.05f;
    int smjer = 1;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is  <  sf::Event::Closed  >())
                window.close();
        }
        // Kretanje miša
        brzina = brzina + ubrzanje;
        misX = misX + smjer * brzina;

        float levaGranica = STO_X;
        float desnaGranica = STO_X + STO_W - 100.f;

        if (misX >= desnaGranica) {
            misX = desnaGranica;
            smjer = -1;
            brzina = 0.f;
        }
        if (misX <= levaGranica) {
            misX = levaGranica;
            smjer = 1;
            brzina = 0.f;
        }
        pulseTimer += 0.05f;

        window.clear(sf::Color(10, 15, 10));

        nacrtajPozadinu();
        nacrtajGlavnuSlikuMolekula();

        for (int i = 0; i < BROJ_MALIH_SLIKA; i++) {
            nacrtajMaluSliku(maleSlike[i], i);
        }

        nacrtajSto();
        nacrtajDestilaciju(DESTILACIJA_X, DESTILACIJA_Y);
        nacrtajMikroskop(MIKROSKOP_X, MIKROSKOP_Y);
        nacrtajStalakIEpruvete(nivoi);
        nacrtajUpozorenjeTraku(UPOZORENJE_Y);
        nacrtajMenzuru(STO_X + STO_W - 45.f, STO_Y - 120.f);
        nacrtajMis(misX, misY, smjer);

        window.display();
        sf::sleep(sf::milliseconds(16));
    }
    return 0;
}