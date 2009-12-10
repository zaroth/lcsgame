/*

Copyright (c) 2002,2003,2004 by Tarn Adams                                            //
                                                                                      //
This file is part of Liberal Crime Squad.                                             //
                                                                                    //
    Liberal Crime Squad is free software; you can redistribute it and/or modify     //
    it under the terms of the GNU General Public License as published by            //
    the Free Software Foundation; either version 2 of the License, or               //
    (at your option) any later version.                                             //
                                                                                    //
    Liberal Crime Squad is distributed in the hope that it will be useful,          //
    but WITHOUT ANY WARRANTY; without even the implied warranty of                  //
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.   See the                  //
    GNU General Public License for more details.                                    //
                                                                                    //
    You should have received a copy of the GNU General Public License               //
    along with Liberal Crime Squad; if not, write to the Free Software              //
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA   02111-1307   USA     //
*/

/*
        This file was created by Chris Johnson (grundee@users.sourceforge.net)
        by copying code from game.cpp.
        To see descriptions of files and functions, see the list at
        the bottom of includes.h in the top src folder.
*/

//#include <includes.h>
#include <externs.h>



/* supporting function -- gives "standard" prices on guns */
long gun_price(enum Weapons type) {
    double price = 0;

    switch(type) {
    case WEAPON_NONE:
    case WEAPON_SHANK:
        /*case WEAPON_CHAIN:
        case WEAPON_NIGHTSTICK:
        case WEAPON_CROWBAR:
        case WEAPON_BASEBALLBAT:
        case WEAPON_GAVEL:
        case WEAPON_CROSS:
        case WEAPON_STAFF:
        case WEAPON_TORCH:
        case WEAPON_SPRAYCAN:
        case WEAPON_PITCHFORK:*/ // not priced yet
        price = 1;
        break;

    case WEAPON_KNIFE:
    case WEAPON_SYRINGE:
    case WEAPON_MOLOTOV:
        price = 10;
        break;

    case WEAPON_SWORD:
    case WEAPON_DAISHO:
    case WEAPON_HAMMER:
    case WEAPON_MAUL:
    case WEAPON_AXE:
        price = 100;
        break;

    case WEAPON_REVOLVER_38:
        price = 200;
        break;

    case WEAPON_SEMIPISTOL_9MM:
    case WEAPON_SEMIPISTOL_45:
        price = 300;
        break;

    case WEAPON_SHOTGUN_PUMP:
        price = 400;
        break;

    case WEAPON_REVOLVER_44:
        price = 500;
        break;

    case WEAPON_DESERT_EAGLE:
    case WEAPON_SEMIRIFLE_AR15:
        price = 800;
        break;

    case WEAPON_SMG_MP5:
    case WEAPON_FLAMETHROWER:
        price = 1100;
        break;

    case WEAPON_AUTORIFLE_AK47:
        price = 1400;
        break;

    case WEAPON_AUTORIFLE_M16:
        price = 1500;
        break;

    case WEAPON_CARBINE_M4:
        price = 1500;
        break;
    }

    // Handle illegal weapons by making them more expensive

    // Note no break statements: the more illegal guns are,
    // the more the price of the most dangerous ones spirals
    // upward
    switch(type) {
    case WEAPON_SMG_MP5:
    case WEAPON_CARBINE_M4:
    case WEAPON_AUTORIFLE_M16:
    case WEAPON_AUTORIFLE_AK47:
        if(law[LAW_GUNCONTROL] != -2)
            price *= 2.0;

    case WEAPON_SEMIRIFLE_AR15:
    case WEAPON_REVOLVER_44:
    case WEAPON_DESERT_EAGLE:
        if(law[LAW_GUNCONTROL] > -1)
            price *= 2.0;

    case WEAPON_SEMIPISTOL_9MM:
    case WEAPON_SEMIPISTOL_45:
        if(law[LAW_GUNCONTROL] > 0)
            price *= 2.0;

    case WEAPON_REVOLVER_38:
        if(law[LAW_GUNCONTROL] > 1)
            price *= 2.0;
    }

    return static_cast<int>(price);
}


/* active squad visits the hospital */
void hospital(int loc) {
    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        erase();

        locheader();
        printparty();

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(10, 1);
        addstr("F - Go in and fix up Conservative wounds");

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(12, 1);
        addstr("Enter - Leave");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(13, 1);
        addstr("# - Check the status of a squad Liberal");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(14, 1);
        addstr("0 - Show the squad's Liberal status");

        int c = getch();
        translategetch(c);

        if(c == 10)
            break;

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

        if(c == 'f') {
            for(int p = 5; p >= 0; p--) {
                if(activesquad->squad[p] == NULL)
                    continue;

                hospitalize(loc, *activesquad->squad[p]);
            }

            break;
        }
    } while(1);
}

/* select a gun for arms dealership */
char gunselect(Creature *cr, short &gun, bool legal = 1) {
    gun = -1;

    vector<enum Weapons> guntype;

    for(int a = 0; a < WEAPONNUM; a++) {
        switch(a) {
        /* list here any guns to be sold at gunshop */
        case WEAPON_SMG_MP5:
            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_CARBINE_M4:
            if(!legal)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_AUTORIFLE_M16:
            if(!legal)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_AUTORIFLE_AK47:
            if(legal && law[LAW_GUNCONTROL] != -2)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_SEMIRIFLE_AR15:
            if(legal && law[LAW_GUNCONTROL] > -1)
                break;

            if(!legal)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_REVOLVER_44:
            if(legal && law[LAW_GUNCONTROL] > -1)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_SEMIPISTOL_9MM:
        case WEAPON_SEMIPISTOL_45:
            if(legal && law[LAW_GUNCONTROL] > 0)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_REVOLVER_38:
            if(legal && law[LAW_GUNCONTROL] > 1)
                break;

            if(!legal)
                break;

            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        case WEAPON_SHOTGUN_PUMP:
            guntype.push_back(static_cast<enum Weapons>(a));
            break;

        default:
            break;
        }
    }

    int page = 0;

    char str[200];

    do {
        erase();

        set_color(COLOR_WHITE, COLOR_BLACK, 1);
        move(0, 0);
        addstr("Which weapon will ");
        addstr(cr->name);
        addstr(" buy?");
        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(1, 0);
        addstr("----PRODUCT NAME-----------------------PRICE------------------------------------");

        int y = 2;

        for(int p = page * 19; p < guntype.size() && p < page * 19 + 19; p++) {
            int price = gun_price(guntype[p]);

            if(funds < price)
                set_color(COLOR_BLACK, COLOR_BLACK, 1);
            else
                set_color(COLOR_WHITE, COLOR_BLACK, 0);

            move(y, 0);
            addch(y + 'A' - 2);
            addstr(" - ");
            getweaponfull(str, guntype[p]);
            addstr(str);

            move(y, 39);
            itoa(price, str, 10);
            addch('$');
            addstr(str);

            y++;
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(22, 0);
        addstr("Press a Letter to select a Weapon");
        move(23, 0);
        addpagestr();
        move(24, 0);
        addstr("Enter - ");
        addstr(cr->name);
        addstr(" needs no weapon");

        refresh();

        int c = getch();
        translategetch(c);

        //PAGE UP
        if((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
            page--;

        //PAGE DOWN
        if((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT) && (page + 1) * 19 < guntype.size())
            page++;

        if(c >= 'a' && c <= 's') {
            int p = page * 19 + (int)(c - 'a');

            if(p < guntype.size()) {
                int price = gun_price(guntype[p]);

                if(price < funds) {
                    gun = guntype[p];
                    funds -= price;
                    stat_spent += price;
                    moneylost_goods += price;
                }

                return 1;
            }
        }

        if(c == 10)
            break;
    } while(1);

    return 0;
}

/* active squad visits the arms dealer */
void armsdealer(int loc) {
    short buyer = 0;
    short in_gunshop = 0;

    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        erase();

        locheader();
        printparty();

        move(8, 60);
        addstr("Buyer: ");
        addstr(activesquad->squad[buyer]->name);

        if(in_gunshop == 2) {
            if(year < 2100) {
                if(funds >= 25)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);
                addstr("P - Buy Shotgun Shells        ($25)");

                if(funds >= 25)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 40);
                addstr("T - Buy a .38 Speedloader     ($25)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);
                addstr("N - Buy a 9mm Pistol Magazine ($40)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 40);
                addstr("F - Buy a .45 Pistol Magazine ($40)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);
                addstr("M - Buy a .44 Speedloader     ($40)");

                if(funds >= 50)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 40);
                addstr("S - Buy a 9mm SMG Magazine    ($50)");

                if(funds >= 50)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(13, 1);
                addstr("R - Buy an Assault Rifle Mag  ($50)");
            } else {
                if(funds >= 25)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);
                addstr("P - Buy a Shotgun Plasma Pack ($25)");

                if(funds >= 25)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 40);
                addstr("T - Buy a .38 Slug Magazine   ($25)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);
                addstr("N - Buy a 9mm Powerpack       ($40)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 40);
                addstr("F - Buy a .45 Powerpack       ($40)");

                if(funds >= 40)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);
                addstr("M - Buy a .44 Heavy Slug Mag  ($40)");

                if(funds >= 50)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 40);
                addstr("S - Buy a SMG Powerpack       ($50)");

                if(funds >= 50)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(13, 1);
                addstr("R - Buy a Rifle Powerpack     ($50)");
            }


            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Done buying Liberal clips");
        } else if(in_gunshop == 1) {
            short gunbought;

            if(gunselect(activesquad->squad[buyer], gunbought, 0)) {
                weaponst swap = activesquad->squad[buyer]->weapon;
                activesquad->squad[buyer]->weapon.type = gunbought;
                activesquad->squad[buyer]->weapon.ammo = 0;

                if(swap.type != WEAPON_NONE) {
                    itemst *newi = new itemst;
                    newi->type = ITEM_WEAPON;
                    newi->weapon = swap;

                    if(swap.type == WEAPON_MOLOTOV &&
                            activesquad->squad[buyer]->clip[CLIP_MOLOTOV]) {
                        newi->number = 1 + activesquad->squad[buyer]->clip[CLIP_MOLOTOV];
                        activesquad->squad[buyer]->clip[CLIP_MOLOTOV] = 0;
                    }

                    location[activesquad->squad[0]->base]->loot.push_back(newi);
                }

                //DROP ALL CLIPS THAT DON'T WORK
                for(int cl = 0; cl < CLIPNUM; cl++) {
                    if(cl == ammotype(activesquad->squad[buyer]->weapon.type))
                        continue;

                    for(int p2 = 0; p2 < activesquad->squad[buyer]->clip[cl]; p2++) {
                        itemst *newi = new itemst;
                        newi->type = ITEM_CLIP;
                        newi->cliptype = cl;
                        location[activesquad->squad[0]->base]->loot.push_back(newi);
                    }

                    activesquad->squad[buyer]->clip[cl] = 0;
                }
            }

            in_gunshop = 0;
            continue;
        } else {
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(10, 1);
            addstr("G - Buy a Liberal gun");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(10, 40);
            addstr("C - Buy Liberal clips");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(11, 1);
            addstr("E - Look over Equipment");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Leave");
        }

        if(partysize >= 2)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(16, 1);
        addstr("B - Choose a buyer");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 1);
        addstr("0 - Show the squad's Liberal status");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 40);
        addstr("# - Check the status of a squad Liberal");

        int c = getch();
        translategetch(c);

        if(in_gunshop == 2) {
            int clipbought = -1;

            if(funds >= 25 && c == 't') {
                clipbought = CLIP_38;
                funds -= 25;
                stat_spent += 25;
                moneylost_goods += 25;
            }

            if(funds >= 40 && c == 'm') {
                clipbought = CLIP_44;
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;
            }

            if(funds >= 40 && c == 'n') {
                clipbought = CLIP_9;
                funds -= 30;
                stat_spent += 30;
                moneylost_goods += 30;
            }

            if(funds >= 40 && c == 'f') {
                clipbought = CLIP_45;
                funds -= 30;
                stat_spent += 30;
                moneylost_goods += 30;
            }

            if(funds >= 25 && c == 'p') {
                clipbought = CLIP_BUCKSHOT;
                funds -= 25;
                stat_spent += 25;
                moneylost_goods += 25;
            }

            if(funds >= 50 && c == 'r') {
                clipbought = CLIP_ASSAULT;
                funds -= 50;
                stat_spent += 50;
                moneylost_goods += 50;
            }

            if(funds >= 50 && c == 's') {
                clipbought = CLIP_SMG;
                funds -= 50;
                stat_spent += 50;
                moneylost_goods += 50;
            }

            char conf = 1;

            if(ammotype(activesquad->squad[buyer]->weapon.type) == clipbought && clipbought != -1) {
                if(activesquad->squad[buyer]->clip[clipbought] < 9) {
                    activesquad->squad[buyer]->clip[clipbought]++;
                    conf = 0;
                }
            }

            if(conf && clipbought != -1) {
                itemst *newi = new itemst;
                newi->type = ITEM_CLIP;
                newi->cliptype = clipbought;
                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }

            if(c == 10)
                in_gunshop = 0;
        } else {
            if(c == 10)
                break;

            if(c == 'c')
                in_gunshop = 2;

            if(c == 'g')
                in_gunshop = 1;

            if(c == 'e')
                equip(location[activesquad->squad[0]->location]->loot, -1);
        }

        if(c == 'b')
            choose_buyer(buyer);

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

    } while(1);
}


/* active squad visits the pawn shop */
void pawnshop(int loc) {
    short buyer = 0;
    short in_tools = 0;
    short in_gunshop = 0;
    short in_fence = 0;
    int l;

    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        erase();

        locheader();
        printparty();

        move(8, 60);
        addstr("Buyer: ");
        addstr(activesquad->squad[buyer]->name);

        if(in_fence == 1) {
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(10, 1);
            addstr("E - Look over Equipment");

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 40);
            addstr("F - Pawn Selectively");

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 1);
            addstr("W - Pawn all Weapons");

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 40);
            addstr("A - Pawn all Ammunition");

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(12, 1);
            addstr("C - Pawn all Clothes");

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(12, 40);
            addstr("L - Pawn all Loot");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Done pawning");
        } else if(in_gunshop == 2) {
            if(year < 2100) {
                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);

                if(law[LAW_GUNCONTROL] < 2)
                    addstr("P - Buy Shotgun Shells        ($20)");

                if(funds >= 15)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 40);

                if(law[LAW_GUNCONTROL] < 2)
                    addstr("T - Buy a .38 Speedloader     ($15)");

                if(funds >= 30)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("N - Buy a 9mm Pistol Magazine ($30)");

                if(funds >= 30)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 40);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("F - Buy a .45 Pistol Magazine ($30)");

                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("M - Buy a .44 Speedloader     ($20)");

                if(funds >= 35)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 40);

                if(law[LAW_GUNCONTROL] < 0)
                    addstr("R - Buy a Rifle/Aslt.Rifle Mag($35)");

                if(funds >= 35)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(13, 1);

                if(law[LAW_GUNCONTROL] < -1)
                    addstr("S - Buy a 9mm SMG Magazine    ($35)");
            } else {
                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);

                if(law[LAW_GUNCONTROL] < 2)
                    addstr("P - Buy a Shotgun Plasma Pack ($20)");

                if(funds >= 15)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 40);

                if(law[LAW_GUNCONTROL] < 2)
                    addstr("T - Buy a .38 Slug Magazine   ($15)");

                if(funds >= 30)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("N - Buy a 9mm Powerpack       ($30)");

                if(funds >= 30)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 40);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("F - Buy a .45 Powerpack       ($30)");

                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);

                if(law[LAW_GUNCONTROL] < 1)
                    addstr("M - Buy a .44 Heavy Slug Mag  ($20)");

                if(funds >= 35)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 40);

                if(law[LAW_GUNCONTROL] < 0)
                    addstr("R - Buy a Rifle Powerpack     ($35)");

                if(funds >= 35)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(13, 1);

                if(law[LAW_GUNCONTROL] < -1)
                    addstr("S - Buy a SMG Powerpack       ($35)");
            }


            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Done buying Liberal clips");
        } else if(in_gunshop == 1) {
            if(year < 2100) {
                if(law[LAW_GUNCONTROL] < 2) {
                    if(funds >= 400)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(10, 1);
                    addstr("G - Buy a Shotgun             ($400)");

                    if(funds >= 150)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(10, 40);
                    addstr("T - Buy a .38 Revolver        ($150)");
                }

                if(law[LAW_GUNCONTROL] < 1) {
                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(11, 1);
                    addstr("N - Buy a 9mm Semi-automatic  ($300)");

                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(11, 40);
                    addstr("F - Buy a .45 Semi-automatic  ($300)");

                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(12, 1);
                    addstr("M - Buy a .44 Magnum          ($300)");
                }

                if(law[LAW_GUNCONTROL] < 0) {
                    if(funds >= 350)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(12, 40);
                    addstr("R - Buy a Civilian AR-15      ($350)");
                }

                if(law[LAW_GUNCONTROL] == -2) {
                    if(funds >= 1200)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(13, 1);
                    addstr("S - Buy a 9mm MP5 SMG         ($1200)");

                    if(funds >= 1400)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(13, 40);
                    addstr("C - Buy an M4 Carbine         ($1400)");

                    if(funds >= 1500)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(14, 1);
                    addstr("A - Buy an M16                ($1500)");

                    if(funds >= 1500)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(14, 40);
                    addstr("K - Buy an AK-47              ($1500)");
                }
            } else {
                if(law[LAW_GUNCONTROL] < 2) {
                    if(funds >= 400)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(10, 1);
                    addstr("G - Buy a Plasma Shotgun      ($400)");

                    if(funds >= 150)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(10, 40);
                    addstr("T - Buy a .38 Slug Pistol     ($150)");
                }

                if(law[LAW_GUNCONTROL] < 1) {
                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(11, 1);
                    addstr("N - Buy a 9mm Laser Pistol    ($300)");

                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(11, 40);
                    addstr("F - Buy a .45 Laser Pistol    ($300)");

                    if(funds >= 300)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(12, 1);
                    addstr("M - Buy a .44 Slug Pistol     ($300)");
                }

                if(law[LAW_GUNCONTROL] < 0) {
                    if(funds >= 350)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(12, 40);
                    addstr("R - Buy a Personal X-15 Rifle ($350)");
                }

                if(law[LAW_GUNCONTROL] == -2) {
                    if(funds >= 1200)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(13, 1);
                    addstr("S - Buy a Beijing Prince SMG  ($1200)");

                    if(funds >= 1400)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(13, 40);
                    addstr("C - Buy an X4 Laser Carbine   ($1400)");

                    if(funds >= 1500)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(14, 1);
                    addstr("A - Buy an Army X-15 Rifle    ($1500)");

                    if(funds >= 1500)
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    else
                        set_color(COLOR_BLACK, COLOR_BLACK, 1);

                    move(14, 40);
                    addstr("K - Buy a P74 War Laser       ($1500)");
                }
            }

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Done buying Liberal guns");
        } else if(in_tools == 1) {
            if(funds >= 20)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 1);
            addstr("C - Buy a Crowbar              ($20)");

            if(funds >= 20)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 40);
            addstr("S - Buy Spray Paint            ($20)");

            if(funds >= 200)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 1);
            addstr("G - Buy a Guitar              ($200)");

            if(funds >= 40)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 40);
            addstr("M - Buy Bottle, Rag & Gasoline ($40)");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Leave");
        } else {
            if(law[LAW_GUNCONTROL] < 2)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 1);
            addstr("G - Buy a Liberal gun");

            if(law[LAW_GUNCONTROL] < 2)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 40);
            addstr("C - Buy Liberal clips");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(11, 1);
            addstr("T - Buy Liberal tools");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(12, 1);
            addstr("E - Look over Equipment");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);

            if(location[activesquad->squad[0]->base]->loot.size() > 0)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(13, 1);
            addstr("S - Sell something");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(16, 40);
            addstr("Enter - Leave");
        }

        if(partysize >= 2)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(16, 1);
        addstr("B - Choose a buyer");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 1);
        addstr("0 - Show the squad's Liberal status");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 40);
        addstr("# - Check the status of a squad Liberal");

        int c = getch();
        translategetch(c);

        if(in_fence) {
            if(c == 10)
                in_fence = 0;

            if(c == 'e')
                equip(location[activesquad->squad[0]->location]->loot, -1);

            if((c == 'w' || c == 'c' || c == 'l' || c == 'a' || c == 'f') &&
                    location[activesquad->squad[0]->base]->loot.size() > 0) {
                int fenceamount = 0;

                if(c == 'f')
                    fenceamount = fenceselect();
                else {
                    int fenceweapon[WEAPONNUM];
                    int fencearmor[ARMORNUM];
                    int fenceclip[CLIPNUM];
                    int fenceloot[LOOTNUM];

                    memset(fenceweapon, 0, WEAPONNUM * sizeof(int));
                    memset(fencearmor, 0, ARMORNUM * sizeof(int));
                    memset(fenceclip, 0, CLIPNUM * sizeof(int));
                    memset(fenceloot, 0, LOOTNUM * sizeof(int));

                    for(l = location[activesquad->squad[0]->base]->loot.size() - 1; l >= 0; l--) {
                        switch(location[activesquad->squad[0]->base]->loot[l]->type) {
                        case ITEM_WEAPON:
                            fenceweapon[location[activesquad->squad[0]->base]->loot[l]->weapon.type] += location[activesquad->squad[0]->base]->loot[l]->number;
                            break;

                        case ITEM_ARMOR:
                            if(location[activesquad->squad[0]->base]->loot[l]->armor.quality != '1')
                                break;

                            if(location[activesquad->squad[0]->base]->loot[l]->armor.flag != 0)
                                break;

                            fencearmor[location[activesquad->squad[0]->base]->loot[l]->armor.type] += location[activesquad->squad[0]->base]->loot[l]->number;
                            break;

                        case ITEM_CLIP:
                            fenceclip[location[activesquad->squad[0]->base]->loot[l]->cliptype] += location[activesquad->squad[0]->base]->loot[l]->number;
                            break;

                        case ITEM_LOOT:
                            fenceloot[location[activesquad->squad[0]->base]->loot[l]->loottype] += location[activesquad->squad[0]->base]->loot[l]->number;
                            break;
                        }
                    }

                    if(c != 'w')
                        memset(fenceweapon, 0, WEAPONNUM * sizeof(int));

                    if(c != 'c')
                        memset(fencearmor, 0, ARMORNUM * sizeof(int));

                    if(c != 'a')
                        memset(fenceclip, 0, CLIPNUM * sizeof(int));

                    if(c != 'l')
                        memset(fenceloot, 0, LOOTNUM * sizeof(int));
                    else {
                        fenceloot[LOOT_CEOPHOTOS] = 0;
                        fenceloot[LOOT_CEOLOVELETTERS] = 0;
                        fenceloot[LOOT_CEOTAXPAPERS] = 0;
                        fenceloot[LOOT_INTHQDISK] = 0;
                        fenceloot[LOOT_CORPFILES] = 0;
                        fenceloot[LOOT_JUDGEFILES] = 0;
                        fenceloot[LOOT_RESEARCHFILES] = 0;
                        fenceloot[LOOT_PRISONFILES] = 0;
                        fenceloot[LOOT_CABLENEWSFILES] = 0;
                        fenceloot[LOOT_AMRADIOFILES] = 0;
                        fenceloot[LOOT_SECRETDOCUMENTS] = 0;
                        fenceloot[LOOT_POLICERECORDS] = 0;
                    }

                    for(l = location[activesquad->squad[0]->base]->loot.size() - 1; l >= 0; l--) {
                        switch(location[activesquad->squad[0]->base]->loot[l]->type) {
                        case ITEM_WEAPON:
                            if(fenceweapon[location[activesquad->squad[0]->base]->loot[l]->weapon.type] > 0) {
                                long numbersold = fenceweapon[location[activesquad->squad[0]->base]->loot[l]->weapon.type];

                                if(numbersold > location[activesquad->squad[0]->base]->loot[l]->number)
                                    numbersold = location[activesquad->squad[0]->base]->loot[l]->number;

                                fenceweapon[location[activesquad->squad[0]->base]->loot[l]->weapon.type] -= numbersold;
                                location[activesquad->squad[0]->base]->loot[l]->number -= numbersold;
                                fenceamount += fencevalue(*location[activesquad->squad[0]->base]->loot[l]) * numbersold;

                                if(location[activesquad->squad[0]->base]->loot[l]->number == 0) {
                                    delete location[activesquad->squad[0]->base]->loot[l];
                                    location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
                                }
                            }

                            break;

                        case ITEM_ARMOR:
                            if(location[activesquad->squad[0]->base]->loot[l]->armor.quality != '1')
                                break;

                            if(location[activesquad->squad[0]->base]->loot[l]->armor.flag != 0)
                                break;

                            if(fencearmor[location[activesquad->squad[0]->base]->loot[l]->armor.type] > 0) {
                                long numbersold = fencearmor[location[activesquad->squad[0]->base]->loot[l]->armor.type];

                                if(numbersold > location[activesquad->squad[0]->base]->loot[l]->number)
                                    numbersold = location[activesquad->squad[0]->base]->loot[l]->number;

                                fencearmor[location[activesquad->squad[0]->base]->loot[l]->armor.type] -= numbersold;
                                location[activesquad->squad[0]->base]->loot[l]->number -= numbersold;
                                fenceamount += fencevalue(*location[activesquad->squad[0]->base]->loot[l]) * numbersold;

                                if(location[activesquad->squad[0]->base]->loot[l]->number == 0) {
                                    delete location[activesquad->squad[0]->base]->loot[l];
                                    location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
                                }
                            }

                            break;

                        case ITEM_CLIP:
                            if(fenceclip[location[activesquad->squad[0]->base]->loot[l]->cliptype] > 0) {
                                long numbersold = fenceclip[location[activesquad->squad[0]->base]->loot[l]->cliptype];

                                if(numbersold > location[activesquad->squad[0]->base]->loot[l]->number)
                                    numbersold = location[activesquad->squad[0]->base]->loot[l]->number;

                                fenceclip[location[activesquad->squad[0]->base]->loot[l]->cliptype] -= numbersold;
                                location[activesquad->squad[0]->base]->loot[l]->number -= numbersold;
                                fenceamount += fencevalue(*location[activesquad->squad[0]->base]->loot[l]) * numbersold;

                                if(location[activesquad->squad[0]->base]->loot[l]->number == 0) {
                                    delete location[activesquad->squad[0]->base]->loot[l];
                                    location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
                                }
                            }

                            break;

                        case ITEM_LOOT:
                            if(fenceloot[location[activesquad->squad[0]->base]->loot[l]->loottype] > 0) {
                                long numbersold = fenceloot[location[activesquad->squad[0]->base]->loot[l]->loottype];

                                if(numbersold > location[activesquad->squad[0]->base]->loot[l]->number)
                                    numbersold = location[activesquad->squad[0]->base]->loot[l]->number;

                                fenceloot[location[activesquad->squad[0]->base]->loot[l]->loottype] -= numbersold;
                                location[activesquad->squad[0]->base]->loot[l]->number -= numbersold;
                                fenceamount += fencevalue(*location[activesquad->squad[0]->base]->loot[l]) * numbersold;

                                if(location[activesquad->squad[0]->base]->loot[l]->number == 0) {
                                    delete location[activesquad->squad[0]->base]->loot[l];
                                    location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
                                }
                            }

                            break;
                        }
                    }
                }

                if(fenceamount > 0) {
                    set_color(COLOR_WHITE, COLOR_BLACK, 1);

                    char num[20];

                    move(8, 1);
                    addstr("You add $");
                    itoa(fenceamount, num, 10);
                    addstr(num);
                    addstr(" to Liberal Funds.");

                    refresh();
                    getch();

                    funds += fenceamount;
                    stat_funds += fenceamount;
                    moneygained_goods += fenceamount;
                }
            }
        } else if(in_gunshop == 1) {
            int gunbought = -1;

            if(funds >= 150 && c == 't' && law[LAW_GUNCONTROL] < 2) {
                gunbought = WEAPON_REVOLVER_38;
                funds -= 150;
                stat_spent += 150;
                moneylost_goods += 150;
            }

            if(funds >= 300 && c == 'm' && law[LAW_GUNCONTROL] < 1) {
                gunbought = WEAPON_REVOLVER_44;
                funds -= 300;
                stat_spent += 300;
                moneylost_goods += 300;
            }

            if(funds >= 300 && c == 'n' && law[LAW_GUNCONTROL] < 2) {
                gunbought = WEAPON_SEMIPISTOL_9MM;
                funds -= 300;
                stat_spent += 300;
                moneylost_goods += 300;
            }

            if(funds >= 300 && c == 'f' && law[LAW_GUNCONTROL] < 2) {
                gunbought = WEAPON_SEMIPISTOL_45;
                funds -= 300;
                stat_spent += 300;
                moneylost_goods += 300;
            }

            if(funds >= 400 && c == 'g' && law[LAW_GUNCONTROL] < 2) {
                gunbought = WEAPON_SHOTGUN_PUMP;
                funds -= 400;
                stat_spent += 400;
                moneylost_goods += 400;
            }

            if(funds >= 350 && c == 'r' && law[LAW_GUNCONTROL] < 0) {
                gunbought = WEAPON_SEMIRIFLE_AR15;
                funds -= 350;
                stat_spent += 350;
                moneylost_goods += 350;
            }

            if(funds >= 1200 && c == 's' && law[LAW_GUNCONTROL] == -2) {
                gunbought = WEAPON_SMG_MP5;
                funds -= 1200;
                stat_spent += 1200;
                moneylost_goods += 1200;
            }

            if(funds >= 1400 && c == 'c' && law[LAW_GUNCONTROL] == -2) {
                gunbought = WEAPON_CARBINE_M4;
                funds -= 1400;
                stat_spent += 1400;
                moneylost_goods += 1400;
            }

            if(funds >= 1500 && c == 'a' && law[LAW_GUNCONTROL] == -2) {
                gunbought = WEAPON_AUTORIFLE_M16;
                funds -= 1500;
                stat_spent += 1500;
                moneylost_goods += 1500;
            }

            if(funds >= 1500 && c == 'k' && law[LAW_GUNCONTROL] == -2) {
                gunbought = WEAPON_AUTORIFLE_AK47;
                funds -= 1500;
                stat_spent += 1500;
                moneylost_goods += 1500;
            }


            if(gunbought != -1) {
                weaponst swap = activesquad->squad[buyer]->weapon;
                activesquad->squad[buyer]->weapon.type = gunbought;
                activesquad->squad[buyer]->weapon.ammo = 0;

                if(swap.type != WEAPON_NONE) {
                    itemst *newi = new itemst;
                    newi->type = ITEM_WEAPON;
                    newi->weapon = swap;

                    if(swap.type == WEAPON_MOLOTOV &&
                            activesquad->squad[buyer]->clip[CLIP_MOLOTOV]) {
                        newi->number = 1 + activesquad->squad[buyer]->clip[CLIP_MOLOTOV];
                        activesquad->squad[buyer]->clip[CLIP_MOLOTOV] = 0;
                    }

                    location[activesquad->squad[0]->base]->loot.push_back(newi);
                }

                //DROP ALL CLIPS THAT DON'T WORK
                for(int cl = 0; cl < CLIPNUM; cl++) {
                    if(cl == ammotype(activesquad->squad[buyer]->weapon.type))
                        continue;

                    for(int p2 = 0; p2 < activesquad->squad[buyer]->clip[cl]; p2++) {
                        itemst *newi = new itemst;
                        newi->type = ITEM_CLIP;
                        newi->cliptype = cl;
                        location[activesquad->squad[0]->base]->loot.push_back(newi);
                    }

                    activesquad->squad[buyer]->clip[cl] = 0;
                }
            }

            if(c == 10)
                in_gunshop = 0;
        } else if(in_gunshop == 2) {
            int clipbought = -1;

            if(funds >= 15 && c == 't' && law[LAW_GUNCONTROL] < 2) {
                clipbought = CLIP_38;
                funds -= 15;
                stat_spent += 15;
                moneylost_goods += 15;
            }

            if(funds >= 20 && c == 'm' && law[LAW_GUNCONTROL] < 1) {
                clipbought = CLIP_44;
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;
            }

            if(funds >= 30 && c == 'n' && law[LAW_GUNCONTROL] < 2) {
                clipbought = CLIP_9;
                funds -= 30;
                stat_spent += 30;
                moneylost_goods += 30;
            }

            if(funds >= 30 && c == 'f' && law[LAW_GUNCONTROL] < 2) {
                clipbought = CLIP_45;
                funds -= 30;
                stat_spent += 30;
                moneylost_goods += 30;
            }

            if(funds >= 20 && c == 'p' && law[LAW_GUNCONTROL] < 2) {
                clipbought = CLIP_BUCKSHOT;
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;
            }

            if(funds >= 35 && c == 'r' && law[LAW_GUNCONTROL] < 0) {
                clipbought = CLIP_ASSAULT;
                funds -= 35;
                stat_spent += 35;
                moneylost_goods += 35;
            }

            if(funds >= 35 && c == 's' && law[LAW_GUNCONTROL] == -2) {
                clipbought = CLIP_SMG;
                funds -= 35;
                stat_spent += 35;
                moneylost_goods += 35;
            }

            char conf = 1;

            if(ammotype(activesquad->squad[buyer]->weapon.type) == clipbought && clipbought != -1) {
                if(activesquad->squad[buyer]->clip[clipbought] < 9) {
                    activesquad->squad[buyer]->clip[clipbought]++;
                    conf = 0;
                }
            }

            if(conf && clipbought != -1) {
                itemst *newi = new itemst;
                newi->type = ITEM_CLIP;
                newi->cliptype = clipbought;
                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }

            if(c == 10)
                in_gunshop = 0;
        } else if(in_tools == 1) {
            int toolbought = -1;

            if(c == 'c' && funds >= 20) {
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;

                toolbought = WEAPON_CROWBAR;
            }

            if(c == 's' && funds >= 20) {
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;

                toolbought = WEAPON_SPRAYCAN;
            }

            if(c == 'g' && funds >= 200) {
                funds -= 200;
                stat_spent += 200;
                moneylost_goods += 200;

                toolbought = WEAPON_GUITAR;
            }

            if(c == 'm' && funds >= 40) {
                funds -= 40;
                stat_spent += 40;
                moneylost_goods += 40;

                toolbought = WEAPON_MOLOTOV;
            }

            if(toolbought != -1) {
                weaponst swap = activesquad->squad[buyer]->weapon;
                activesquad->squad[buyer]->weapon.type = toolbought;
                activesquad->squad[buyer]->weapon.ammo = 0;

                if(swap.type != WEAPON_NONE) {
                    itemst *newi = new itemst;
                    newi->type = ITEM_WEAPON;
                    newi->weapon = swap;

                    if(swap.type == WEAPON_MOLOTOV &&
                            activesquad->squad[buyer]->clip[CLIP_MOLOTOV]) {
                        newi->number = 1 + activesquad->squad[buyer]->clip[CLIP_MOLOTOV];
                        activesquad->squad[buyer]->clip[CLIP_MOLOTOV] = 0;
                    }

                    location[activesquad->squad[0]->base]->loot.push_back(newi);
                }

                //DROP ALL CLIPS THAT DON'T WORK
                for(int cl = 0; cl < CLIPNUM; cl++) {
                    if(cl == ammotype(activesquad->squad[buyer]->weapon.type))
                        continue;

                    for(int p2 = 0; p2 < activesquad->squad[buyer]->clip[cl]; p2++) {
                        itemst *newi = new itemst;
                        newi->type = ITEM_CLIP;
                        newi->cliptype = cl;
                        location[activesquad->squad[0]->base]->loot.push_back(newi);
                    }

                    activesquad->squad[buyer]->clip[cl] = 0;
                }
            }

            if(c == 10)
                in_tools = 0;
        } else {
            if(c == 10)
                break;

            if(c == 'c' && law[LAW_GUNCONTROL] < 2)
                in_gunshop = 2;

            if(c == 'g' && law[LAW_GUNCONTROL] < 2)
                in_gunshop = 1;

            if(c == 'e')
                equip(location[activesquad->squad[0]->location]->loot, -1);

            if(c == 's')
                in_fence = 1;

            if(c == 't')
                in_tools = 1;
        }

        if(c == 'b')
            choose_buyer(buyer);

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

    } while(1);
}



/* active squad visits the car dealership */
void dealership(int loc) {
    short buyer = 0;
    int l;
    char str[80];

    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        erase();

        locheader();
        printparty();

        vehiclest *car_to_sell = 0;
        int price = 0;

        for(int v = (int)vehicle.size() - 1; v >= 0; v--) {
            if(vehicle[v]->id == activesquad->squad[buyer]->carid)
                car_to_sell = vehicle[v];
        }

        if(!car_to_sell)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(10, 1);
        addstr("G - Get a Liberal car");

        move(11, 1);

        if(car_to_sell) {
            switch(car_to_sell->type) {
            case VEHICLE_VAN:
                price = 8000;
                break;

            case VEHICLE_STATIONWAGON:
                price = 6000;
                break;

            case VEHICLE_SPORTSCAR:
                price = 8000;
                break;

            case VEHICLE_BUG:
                price = 4000;
                break;

            case VEHICLE_PICKUP:
                price = 4000;
                break;

            case VEHICLE_POLICECAR:
                price = 4000;
                break;

            case VEHICLE_TAXICAB:
                price = 4000;
                break;

            case VEHICLE_SUV:
                price = 8000;
                break;

            case VEHICLE_AGENTCAR:
                price = 8000;
                break;

            case VEHICLE_JEEP:
                price = 6000;
                break;
            }

            if(car_to_sell->heat)
                price /= 10;

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            addstr("S - Sell the ");
            getcarfull(str, *car_to_sell);
            addstr(str);
            addstr(" ($");
            itoa(price, str, 10);
            addstr(str);
            addstr(")");
        } else {
            set_color(COLOR_BLACK, COLOR_BLACK, 1);
            addstr("S - Sell a car");
        }

        /*if(car_to_sell && car_to_sell->heat>1 && funds>=500)
           set_color(COLOR_WHITE,COLOR_BLACK,0);
        else
           set_color(COLOR_BLACK,COLOR_BLACK,1);
        move(12,1);
        addstr("P - Repaint car, replace plates and tags ($500)");*/

        if(partysize >= 2)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(16, 1);
        addstr("B - Choose a buyer");

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(16, 40);
        addstr("Enter - Leave");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 1);
        addstr("0 - Show the squad's Liberal status");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 40);
        addstr("# - Check the status of a squad Liberal");

        int c = getch();
        translategetch(c);


        // Leave
        if(c == 10)
            break;

        //Sell the car
        if(c == 's' && car_to_sell) {
            funds += price;
            moneygained_goods += price;
            removecarprefs_pool(car_to_sell->id);
            delete car_to_sell;

            for(int v = (int)vehicle.size() - 1; v >= 0; v--) {
                if(vehicle[v] == car_to_sell) {
                    vehicle.erase(vehicle.begin() + v);
                    break;
                }
            }
        }

        // Get a car
        if(c == 'g' && !car_to_sell) {
            do {
                erase();

                locheader();
                printparty();

                if(funds >= 5000)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);
                addstr("A - Bug ($5000)");
                move(10, 40);
                addstr("B - Pickup Truck ($5000)");

                if(funds >= 10000)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);
                addstr("C - Sports Car ($10000)");
                move(11, 40);
                addstr("D - SUV ($10000)");

                set_color(COLOR_WHITE, COLOR_BLACK, 0);
                move(16, 1);
                addstr("Enter - We don't need a Conservative car");

                if(party_status != -1)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(15, 1);
                addstr("0 - Show the squad's Liberal status");

                if(partysize > 0 && (party_status == -1 || partysize > 1))
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(15, 40);
                addstr("# - Check the status of a squad Liberal");

                int c = getch();
                translategetch(c);

                // Back out
                if(c == 10)
                    break;

                //Picked a car
                if(c >= 'a' && c <= 'd') {
                    if(funds < 5000 || (funds < 10000 && c >= 'c'))
                        continue;

                    int cartype = -1;
                    int carcolor = -1;

                    switch(c) {
                    case 'a':
                        cartype = VEHICLE_BUG;
                        break;

                    case 'b':
                        cartype = VEHICLE_PICKUP;
                        break;

                    case 'c':
                        cartype = VEHICLE_SPORTSCAR;
                        break;

                    case 'd':
                        cartype = VEHICLE_SUV;
                        break;

                    default:
                        continue;
                    }

                    do {
                        erase();

                        locheader();
                        printparty();

                        move(10, 1);
                        addstr("A - Red");
                        move(10, 40);
                        addstr("B - White");
                        move(11, 1);
                        addstr("C - Blue");
                        move(11, 40);
                        addstr("D - Beige");

                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                        move(16, 1);
                        addstr("Enter - These colors are Conservative");

                        if(party_status != -1)
                            set_color(COLOR_WHITE, COLOR_BLACK, 0);
                        else
                            set_color(COLOR_BLACK, COLOR_BLACK, 1);

                        move(15, 1);
                        addstr("0 - Show the squad's Liberal status");

                        if(partysize > 0 && (party_status == -1 || partysize > 1))
                            set_color(COLOR_WHITE, COLOR_BLACK, 0);
                        else
                            set_color(COLOR_BLACK, COLOR_BLACK, 1);

                        move(15, 40);
                        addstr("# - Check the status of a squad Liberal");

                        int c = getch();
                        translategetch(c);

                        // Back out
                        if(c == 10)
                            break;

                        //Picked a color
                        if(c == 'a') {
                            carcolor = VEHICLECOLOR_RED;
                            break;
                        }

                        if(c == 'b') {
                            carcolor = VEHICLECOLOR_WHITE;
                            break;
                        }

                        if(c == 'c') {
                            carcolor = VEHICLECOLOR_BLUE;
                            break;
                        }

                        if(c == 'd') {
                            carcolor = VEHICLECOLOR_BEIGE;
                            break;
                        }

                        if(c == '0')
                            party_status = -1;

                        if(c >= '1' && c <= '6' && activesquad != NULL) {
                            if(activesquad->squad[c - '1'] != NULL) {
                                if(party_status == c - '1')
                                    fullstatus(party_status);
                                else
                                    party_status = c - '1';
                            }
                        }
                    } while(1);

                    if(carcolor != -1) {
                        vehiclest *v = new vehiclest;
                        v->init(cartype);
                        v->color = carcolor;
                        v->myear = year;
                        v->heat  = 0;
                        activesquad->squad[buyer]->carid = v->id;
                        vehicle.push_back(v);

                        switch(cartype) {
                        case VEHICLE_BUG:
                        case VEHICLE_PICKUP:
                            funds -= 5000;
                            moneylost_goods += 5000;
                            break;

                        case VEHICLE_SPORTSCAR:
                        case VEHICLE_SUV:
                            funds -= 10000;
                            moneylost_goods += 10000;

                        default:
                            continue;
                        }

                        break;
                    }
                }

                if(c == '0')
                    party_status = -1;

                if(c >= '1' && c <= '6' && activesquad != NULL) {
                    if(activesquad->squad[c - '1'] != NULL) {
                        if(party_status == c - '1')
                            fullstatus(party_status);
                        else
                            party_status = c - '1';
                    }
                }

            } while(1);
        }

        // Reduce heat
        /*if(c=='p' && car_to_sell && car_to_sell->heat>1 && funds>=500)
        {
           funds-=500;
           moneylost_goods+=500;
           car_to_sell->heat=1;
        }*/

        if(c == 'b')
            choose_buyer(buyer);

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

    } while(1);
}



/* active squad visits the department store */
void deptstore(int loc) {
    short buyer = 0;

    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        int weaponbought = -1;
        int armorbought = -1;

        erase();

        locheader();
        printparty();

        move(8, 60);
        addstr("Buyer: ");
        addstr(activesquad->squad[buyer]->name);

        if(funds >= 400)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(10, 1);
        addstr("C - Buy a Cheap Suit         ($400)");

        if(funds >= 5000)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(11, 1);
        addstr("V - Buy an Expensive Suit   ($5000)");

        if(funds >= 500)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(12, 1);
        addstr("S - Buy a Black Suit         ($500)");

        if(funds >= 400)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(10, 40);
        addstr("H - Buy a Cheap Dress        ($400)");

        if(funds >= 5000)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(11, 40);
        addstr("D - Buy an Expensive Dress  ($5000)");

        if(funds >= 500)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(12, 40);
        addstr("R - Buy a Black Dress        ($500)");

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(15, 1);
        addstr("E - Look over Equipment");
        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(15, 30);
        addstr("Enter - Leave");

        if(partysize >= 2)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 50);
        addstr("B - Choose a buyer");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(14, 1);
        addstr("0 - Show the squad's Liberal status");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(14, 40);
        addstr("# - Check the status of a squad Liberal");

        int c = getch();
        translategetch(c);

        if(c == 10)
            break;

        if(funds >= 400 && c == 'c') {
            armorbought = ARMOR_CHEAPSUIT;
            funds -= 400;
            stat_spent += 400;
            moneylost_goods += 400;
        }

        if(funds >= 5000 && c == 'v') {
            armorbought = ARMOR_EXPENSIVESUIT;
            funds -= 5000;
            stat_spent += 5000;
            moneylost_goods += 5000;
        }

        if(funds >= 500 && c == 's') {
            armorbought = ARMOR_BLACKSUIT;
            funds -= 500;
            stat_spent += 500;
            moneylost_goods += 500;
        }

        if(funds >= 400 && c == 'h') {
            armorbought = ARMOR_CHEAPDRESS;
            funds -= 400;
            stat_spent += 400;
            moneylost_goods += 400;
        }

        if(funds >= 5000 && c == 'd') {
            armorbought = ARMOR_EXPENSIVEDRESS;
            funds -= 5000;
            stat_spent += 5000;
            moneylost_goods += 5000;
        }

        if(funds >= 500 && c == 'r') {
            armorbought = ARMOR_BLACKDRESS;
            funds -= 500;
            stat_spent += 500;
            moneylost_goods += 500;
        }

        if(armorbought != -1) {
            armorst swap = activesquad->squad[buyer]->armor;
            activesquad->squad[buyer]->armor.type = armorbought;
            activesquad->squad[buyer]->armor.flag = 0;
            activesquad->squad[buyer]->armor.quality = '1';

            if(swap.type != ARMOR_NONE) {
                itemst *newi = new itemst;
                newi->type = ITEM_ARMOR;
                newi->armor = swap;
                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }
        }

        if(weaponbought != -1) {
            weaponst swap = activesquad->squad[buyer]->weapon;
            activesquad->squad[buyer]->weapon.type = weaponbought;
            activesquad->squad[buyer]->weapon.ammo = 0;

            if(swap.type != WEAPON_NONE) {
                itemst *newi = new itemst;
                newi->type = ITEM_WEAPON;
                newi->weapon = swap;

                if(swap.type == WEAPON_MOLOTOV &&
                        activesquad->squad[buyer]->clip[CLIP_MOLOTOV]) {
                    newi->number = 1 + activesquad->squad[buyer]->clip[CLIP_MOLOTOV];
                    activesquad->squad[buyer]->clip[CLIP_MOLOTOV] = 0;
                }

                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }

            //DROP ALL CLIPS THAT DON'T WORK
            for(int cl = 0; cl < CLIPNUM; cl++) {
                if(cl == ammotype(activesquad->squad[buyer]->weapon.type))
                    continue;

                for(int p2 = 0; p2 < activesquad->squad[buyer]->clip[cl]; p2++) {
                    itemst *newi = new itemst;
                    newi->type = ITEM_CLIP;
                    newi->cliptype = cl;
                    location[activesquad->squad[0]->base]->loot.push_back(newi);
                }

                activesquad->squad[buyer]->clip[cl] = 0;
            }
        }

        if(c == 'e')
            equip(location[activesquad->squad[0]->location]->loot, -1);

        if(c == 'b')
            choose_buyer(buyer);

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

    } while(1);
}



/* active squad visits the oubliette */
void halloweenstore(int loc) {
    short buyer = 0;
    short in_halloween = 0;

    locatesquad(activesquad, loc);

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    do {
        int weaponbought = -1;
        int armorbought = -1, armorbought2 = -1;

        erase();

        locheader();
        printparty();

        move(8, 60);
        addstr("Buyer: ");
        addstr(activesquad->squad[buyer]->name);

        if(in_halloween == 1) {
            if(funds >= 70)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 1);
            addstr("T - Buy a Trench Coat         ($70)");

            if(funds >= 50)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 1);
            addstr("W - Buy Work Clothes          ($50)");

            if(funds >= 200)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(12, 1);
            addstr("L - Buy a Lab Coat           ($200)");

            if(funds >= 200)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(13, 1);
            addstr("R - Buy a Black Judge's Robe ($200)");

            if(funds >= 200)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 40);
            addstr("C - Buy a Clown Suit         ($200)");

            if(funds >= 350)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 40);
            addstr("G - Buy Bondage Gear         ($350)");

            if(funds >= 15)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(12, 40);
            addstr("M - Buy a Mask                ($15)");

            if(funds >= 90)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(13, 40);
            addstr("O - Buy a Toga                ($90)");

            if(funds >= 1000)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(14, 40);
            addstr("E - Buy an Elephant Suit    ($1000)");

            if(funds >= 1000)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(14, 40);
            addstr("D - Buy a Donkey Suit       ($1000)");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(15, 30);
            addstr("Enter - Done");
        } else if(in_halloween == 2) {
            if(year < 2100) {
                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);
                addstr("K - Buy a Knife                 ($20)");

                if(funds >= 250)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);
                addstr("S - Buy the Sword of Morfiegor ($250)");

                if(funds >= 250)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);
                addstr("A - Buy a Katana and Wakizashi ($250)");
            } else {
                if(funds >= 20)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(10, 1);
                addstr("K - Buy a Vibro-Knife          ($20)");

                if(funds >= 250)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(11, 1);
                addstr("S - Buy a Light Sword          ($250)");

                if(funds >= 250)
                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                else
                    set_color(COLOR_BLACK, COLOR_BLACK, 1);

                move(12, 1);
                addstr("A - Buy the Liberal Twin Swords($250)");
            }

            if(funds >= 250)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(13, 1);
            addstr("H - Buy a Dwarven Hammer       ($250)");

            if(funds >= 250)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(10, 40);
            addstr("M - Buy the Maul of Anrin      ($250)");

            if(funds >= 250)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(11, 40);
            addstr("C - Buy a Silver Cross         ($250)");

            if(funds >= 250)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(12, 40);
            addstr("W - Buy a Wizard's Staff       ($250)");

            if(funds >= 1000)
                set_color(COLOR_WHITE, COLOR_BLACK, 0);
            else
                set_color(COLOR_BLACK, COLOR_BLACK, 1);

            move(13, 40);
            addstr("! - Buy Mithril Mail          ($1000)");

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(15, 30);
            addstr("Enter - Done");
        } else {
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(10, 1);
            addstr("C - Purchase Halloween Costumes");
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(11, 1);
            addstr("M - Purchase Medieval Gear");
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(15, 30);
            addstr("Enter - Leave");
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(15, 1);
        addstr("E - Look over Equipment");

        if(partysize >= 2)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(15, 50);
        addstr("B - Choose a buyer");

        if(party_status != -1)
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(14, 1);
        addstr("0 - Show the squad's Liberal status");

        if(partysize > 0 && (party_status == -1 || partysize > 1))
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
        else
            set_color(COLOR_BLACK, COLOR_BLACK, 1);

        move(14, 40);
        addstr("# - Check the status of a squad Liberal");

        int c = getch();
        translategetch(c);

        if(in_halloween == 1) {
            if(c == 10)
                in_halloween = 0;

            if(c == 't' && funds >= 70) {
                armorbought = ARMOR_TRENCHCOAT;
                funds -= 70;
                stat_spent += 70;
                moneylost_goods += 70;
            }

            if(c == 'w' && funds >= 50) {
                armorbought = ARMOR_WORKCLOTHES;
                funds -= 50;
                stat_spent += 50;
                moneylost_goods += 50;
            }

            if(c == 'l' && funds >= 200) {
                armorbought = ARMOR_LABCOAT;
                funds -= 200;
                stat_spent += 200;
                moneylost_goods += 200;
            }

            if(c == 'r' && funds >= 200) {
                armorbought = ARMOR_BLACKROBE;
                funds -= 200;
                stat_spent += 200;
                moneylost_goods += 200;
            }

            if(c == 'c' && funds >= 200) {
                armorbought = ARMOR_CLOWNSUIT;
                funds -= 200;
                stat_spent += 200;
                moneylost_goods += 200;
            }

            if(c == 'g' && funds >= 350) {
                armorbought = ARMOR_BONDAGEGEAR;
                funds -= 350;
                stat_spent += 350;
                moneylost_goods += 350;
            }

            if(c == 'e' && funds >= 1000) {
                armorbought = ARMOR_ELEPHANTSUIT;
                funds -= 1000;
                stat_spent += 1000;
                moneylost_goods += 1000;
            }

            if(c == 'd' && funds >= 1000) {
                armorbought = ARMOR_DONKEYSUIT;
                funds -= 1000;
                stat_spent += 1000;
                moneylost_goods += 1000;
            }

            if(c == 'm' && funds >= 15) {
                short mask;

                if(maskselect(activesquad->squad[buyer], mask)) {
                    armorbought = ARMOR_MASK;
                    armorbought2 = mask;
                    funds -= 15;
                    stat_spent += 15;
                    moneylost_goods += 15;
                }
            }

            if(c == 'o' && funds >= 90) {
                armorbought = ARMOR_TOGA;
                funds -= 90;
                stat_spent += 90;
                moneylost_goods += 90;
            }
        } else if(in_halloween == 2) {
            if(c == 10)
                in_halloween = 0;

            if(c == 'k' && funds >= 20) {
                weaponbought = WEAPON_KNIFE;
                funds -= 20;
                stat_spent += 20;
                moneylost_goods += 20;
            }

            if(c == 's' && funds >= 250) {
                weaponbought = WEAPON_SWORD;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == 'a' && funds >= 250) {
                weaponbought = WEAPON_DAISHO;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == 'h' && funds >= 250) {
                weaponbought = WEAPON_HAMMER;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == 'm' && funds >= 250) {
                weaponbought = WEAPON_MAUL;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == 'c' && funds >= 250) {
                weaponbought = WEAPON_CROSS;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == 'w' && funds >= 250) {
                weaponbought = WEAPON_STAFF;
                funds -= 250;
                stat_spent += 250;
                moneylost_goods += 250;
            }

            if(c == '!' && funds >= 1000) {
                armorbought = ARMOR_MITHRIL;
                funds -= 1000;
                stat_spent += 1000;
                moneylost_goods += 1000;
            }
        } else {
            if(c == 10)
                break;

            if(c == 'c')
                in_halloween = 1;

            if(c == 'm')
                in_halloween = 2;
        }

        if(armorbought != -1) {
            armorst swap = activesquad->squad[buyer]->armor;
            activesquad->squad[buyer]->armor.type = armorbought;
            activesquad->squad[buyer]->armor.subtype = armorbought2;
            activesquad->squad[buyer]->armor.flag = 0;
            activesquad->squad[buyer]->armor.quality = '1';

            if(swap.type != ARMOR_NONE) {
                itemst *newi = new itemst;
                newi->type = ITEM_ARMOR;
                newi->armor = swap;
                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }
        }

        if(weaponbought != -1) {
            weaponst swap = activesquad->squad[buyer]->weapon;
            activesquad->squad[buyer]->weapon.type = weaponbought;
            activesquad->squad[buyer]->weapon.ammo = 0;

            if(swap.type != WEAPON_NONE) {
                itemst *newi = new itemst;
                newi->type = ITEM_WEAPON;
                newi->weapon = swap;

                if(swap.type == WEAPON_MOLOTOV &&
                        activesquad->squad[buyer]->clip[CLIP_MOLOTOV]) {
                    newi->number = 1 + activesquad->squad[buyer]->clip[CLIP_MOLOTOV];
                    activesquad->squad[buyer]->clip[CLIP_MOLOTOV] = 0;
                }

                location[activesquad->squad[0]->base]->loot.push_back(newi);
            }

            //DROP ALL CLIPS THAT DON'T WORK
            for(int cl = 0; cl < CLIPNUM; cl++) {
                if(cl == ammotype(activesquad->squad[buyer]->weapon.type))
                    continue;

                for(int p2 = 0; p2 < activesquad->squad[buyer]->clip[cl]; p2++) {
                    itemst *newi = new itemst;
                    newi->type = ITEM_CLIP;
                    newi->cliptype = cl;
                    location[activesquad->squad[0]->base]->loot.push_back(newi);
                }

                activesquad->squad[buyer]->clip[cl] = 0;
            }
        }

        if(c == 'e')
            equip(location[activesquad->squad[0]->location]->loot, -1);

        if(c == 'b')
            choose_buyer(buyer);

        if(c == '0')
            party_status = -1;

        if(c >= '1' && c <= '6' && activesquad != NULL) {
            if(activesquad->squad[c - '1'] != NULL) {
                if(party_status == c - '1')
                    fullstatus(party_status);
                else
                    party_status = c - '1';
            }
        }

    } while(1);
}



/* oubliette - buy a mask */
char maskselect(Creature *cr, short &mask) {
    mask = -1;

    vector<int> masktype;

    for(int a = 0; a < MASKNUM; a++) {
        switch(a) {
        case MASK_JESUS:
        case MASK_COLEMAN_GARY:
        case MASK_MADONNA:
        case MASK_SPEARS:
        case MASK_EMINEM:
        case MASK_AGUILERA:
        case MASK_WAHLBERG:
        case MASK_IGGYPOP:
        case MASK_CASH:
        case MASK_BINLADEN:
        case MASK_LORDS:
        case MASK_SHIELDS:
        case MASK_JACKSON_MICHAEL:
        case MASK_CRUTHERS:
        case MASK_KING_DON:
            break; /* comment this break to allow all masks, leave it in to hide some for surprises */

        default:
            masktype.push_back(a);
        }
    }

    int page = 0;

    char str[200];

    do {
        erase();

        set_color(COLOR_WHITE, COLOR_BLACK, 1);
        move(0, 0);
        addstr("Which mask will ");
        addstr(cr->name);
        addstr(" buy?");
        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(1, 0);
        addstr("----PRODUCT NAME-----------------------DESCRIPTION------------------------------");

        int y = 2;

        for(int p = page * 19; p < masktype.size() && p < page * 19 + 19; p++) {
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(y, 0);
            addch(y + 'A' - 2);
            addstr(" - ");
            getarmorfull(str, ARMOR_MASK, masktype[p]);
            addstr(str);

            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            move(y, 39);
            getmaskdesc(str, masktype[p]);
            addstr(str);

            y++;
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(22, 0);
        addstr("Press a Letter to select a Mask");
        move(23, 0);
        addpagestr();
        move(24, 0);
        addstr("Z - Surprise ");
        addstr(cr->name);
        addstr(" With a Random Mask");

        refresh();

        int c = getch();
        translategetch(c);

        //PAGE UP
        if((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
            page--;

        //PAGE DOWN
        if((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT) && (page + 1) * 19 < masktype.size())
            page++;

        if(c >= 'a' && c <= 's') {
            int p = page * 19 + (int)(c - 'a');

            if(p < masktype.size()) {
                mask = masktype[p];
                return 1;
            }
        }

        if(c == 'z') {
            mask = LCSrandom(MASKNUM);
            return 1;
        }

        if(c == 10)
            break;
    } while(1);

    return 0;
}



/* pick stuff to fence */
int fenceselect(void) {
    int ret = 0;

    consolidateloot(location[activesquad->squad[0]->base]->loot);

    int page = 0;

    vector<int> selected;
    selected.resize(location[activesquad->squad[0]->base]->loot.size());

    for(int s = 0; s < selected.size(); s++)
        selected[s] = 0;

    do {
        erase();

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(0, 0);
        addstr("What will you sell?");

        if(ret > 0) {
            char num[20];
            move(0, 30);
            addstr("Estimated Liberal Amount: $");
            itoa(ret, num, 10);
            addstr(num);
        }

        printparty();

        int x = 1, y = 10;
        char str[200], str2[200];

        for(int l = page * 18; l < location[activesquad->squad[0]->base]->loot.size() && l < page * 18 + 18; l++) {
            if(selected[l])
                set_color(COLOR_GREEN, COLOR_BLACK, 1);
            else
                set_color(COLOR_WHITE, COLOR_BLACK, 0);

            if(location[activesquad->squad[0]->base]->loot[l]->type == ITEM_WEAPON) {
                getweaponfull(str2, location[activesquad->squad[0]->base]->loot[l]->weapon.type);

                if(location[activesquad->squad[0]->base]->loot[l]->weapon.ammo > 0) {
                    char num[20];
                    itoa(location[activesquad->squad[0]->base]->loot[l]->weapon.ammo, num, 10);
                    strcat(str2, " (");
                    strcat(str2, num);
                    strcat(str2, ")");
                }
            }

            if(location[activesquad->squad[0]->base]->loot[l]->type == ITEM_ARMOR)
                getarmorfull(str2, location[activesquad->squad[0]->base]->loot[l]->armor, 0);

            if(location[activesquad->squad[0]->base]->loot[l]->type == ITEM_CLIP)
                getclip(str2, location[activesquad->squad[0]->base]->loot[l]->cliptype);

            if(location[activesquad->squad[0]->base]->loot[l]->type == ITEM_LOOT)
                getloot(str2, location[activesquad->squad[0]->base]->loot[l]->loottype);

            if(location[activesquad->squad[0]->base]->loot[l]->number > 1) {
                char num[20];
                strcat(str2, " ");

                if(selected[l] > 0) {
                    itoa(selected[l], num, 10);
                    strcat(str2, num);
                    strcat(str2, "/");
                } else
                    strcat(str2, "x");

                itoa(location[activesquad->squad[0]->base]->loot[l]->number, num, 10);
                strcat(str2, num);
            }

            str[0] = l - page * 18 + 'A';
            str[1] = '\x0';
            strcat(str, " - ");
            strcat(str, str2);

            move(y, x);
            addstr(str);

            x += 26;

            if(x > 53) {
                x = 1;
                y++;
            }
        }

        //PAGE UP
        set_color(COLOR_WHITE, COLOR_BLACK, 0);

        if(page > 0) {
            move(17, 1);
            addprevpagestr();
        }

        //PAGE DOWN
        if((page + 1) * 18 < location[activesquad->squad[0]->base]->loot.size()) {
            move(17, 53);
            addnextpagestr();
        }

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        move(23, 1);
        addstr("Press a letter to select an item to sell.");
        move(24, 1);
        addstr("X - Done");

        refresh();

        int c = getch();
        translategetch(c);

        if(c >= 'a' && c <= 'r') {
            int slot = c - 'a' + page * 18;

            if(slot >= 0 && slot < location[activesquad->squad[0]->base]->loot.size()) {
                if(selected[slot]) {
                    ret -= fencevalue(*location[activesquad->squad[0]->base]->loot[slot]) * selected[slot];
                    selected[slot] = 0;
                } else {
                    char bad = 0;

                    if(location[activesquad->squad[0]->base]->loot[slot]->type == ITEM_ARMOR) {
                        //if(location[activesquad->squad[0]->base]->loot[slot]->armor.quality!='1')bad=1;
                        if(location[activesquad->squad[0]->base]->loot[slot]->armor.flag != 0)
                            bad = 1;
                    }

                    if(bad) {
                        printparty();

                        move(8, 15);
                        set_color(COLOR_WHITE, COLOR_BLACK, 1);
                        addstr(" You can't sell damaged goods.");

                        refresh();
                        getch();
                    } else {
                        if(location[activesquad->squad[0]->base]->loot[slot]->number > 1) {
                            selected[slot] = 1;

                            printparty();

                            move(8, 15);
                            set_color(COLOR_WHITE, COLOR_BLACK, 1);
                            addstr("       How many?          ");

                            refresh();

                            char str[100];

                            keypad(stdscr, FALSE);
                            raw_output(FALSE);
                            echo();
                            curs_set(1);
                            mvgetstr(8, 30, str);
                            curs_set(0);
                            noecho();
                            raw_output(TRUE);
                            keypad(stdscr, TRUE);

                            selected[slot] = atoi(str);

                            if(selected[slot] < 0)
                                selected[slot] = 0;
                            else if(selected[slot] > location[activesquad->squad[0]->base]->loot[slot]->number)
                                selected[slot] = location[activesquad->squad[0]->base]->loot[slot]->number;
                        } else
                            selected[slot] = 1;

                        ret += fencevalue(*location[activesquad->squad[0]->base]->loot[slot]) * selected[slot];
                    }
                }
            }
        }

        if(c == 'x')
            break;

        //PAGE UP
        if((c == interface_pgup || c == KEY_UP || c == KEY_LEFT) && page > 0)
            page--;

        //PAGE DOWN
        if((c == interface_pgdn || c == KEY_DOWN || c == KEY_RIGHT) && (page + 1) * 18 < location[activesquad->squad[0]->base]->loot.size())
            page++;

    } while(1);

    for(int l = location[activesquad->squad[0]->base]->loot.size() - 1; l >= 0; l--) {
        if(selected[l] > 0) {
            location[activesquad->squad[0]->base]->loot[l]->number -= selected[l];

            if(location[activesquad->squad[0]->base]->loot[l]->number <= 0) {
                delete location[activesquad->squad[0]->base]->loot[l];
                location[activesquad->squad[0]->base]->loot.erase(location[activesquad->squad[0]->base]->loot.begin() + l);
            }
        }
    }

    return ret;
}



/* value of stuff to fence */
int fencevalue(itemst &it) {
    int fenceamount = 0;

    switch(it.type) {
    case ITEM_WEAPON:
        switch(it.weapon.type) {
        case WEAPON_CROWBAR:
            fenceamount = 10;
            break;

        case WEAPON_BASEBALLBAT:
            fenceamount = 20;
            break;

        case WEAPON_KNIFE:
            fenceamount = 10;
            break;

        case WEAPON_SHANK:
            fenceamount = 5;
            break;

        case WEAPON_SYRINGE:
            fenceamount = 10;
            break;

        case WEAPON_REVOLVER_38:
            fenceamount = 50;
            break;

        case WEAPON_REVOLVER_44:
            fenceamount = 100;
            break;

        case WEAPON_DESERT_EAGLE:
            fenceamount = 250;
            break;

        case WEAPON_SEMIPISTOL_9MM:
            fenceamount = 100;
            break;

        case WEAPON_SEMIPISTOL_45:
            fenceamount = 100;
            break;

        case WEAPON_SMG_MP5:
            fenceamount = 450;
            break;

        case WEAPON_SEMIRIFLE_AR15:
            fenceamount = 250;
            break;

        case WEAPON_AUTORIFLE_M16:
            fenceamount = 500;
            break;

        case WEAPON_AUTORIFLE_AK47:
            fenceamount = 500;
            break;

        case WEAPON_CARBINE_M4:
            fenceamount = 400;
            break;

        case WEAPON_SHOTGUN_PUMP:
            fenceamount = 150;
            break;

        case WEAPON_DAISHO:
            fenceamount = 100;
            break;

        case WEAPON_HAMMER:
            fenceamount = 100;
            break;

        case WEAPON_MAUL:
            fenceamount = 100;
            break;

        case WEAPON_CROSS:
            fenceamount = 100;
            break;

        case WEAPON_STAFF:
            fenceamount = 100;
            break;

        case WEAPON_SWORD:
            fenceamount = 100;
            break;

        case WEAPON_GUITAR:
            fenceamount = 100;
            break;

        case WEAPON_CHAIN:
            fenceamount = 10;
            break;

        case WEAPON_NIGHTSTICK:
            fenceamount = 20;
            break;

        case WEAPON_GAVEL:
            fenceamount = 20;
            break;

        case WEAPON_PITCHFORK:
            fenceamount = 20;
            break;

        case WEAPON_TORCH:
            fenceamount = 2;
            break;

        case WEAPON_SPRAYCAN:
            fenceamount = 2;
            break;
        }

        break;

    case ITEM_ARMOR:
        switch(it.armor.type) { // *JDS* all armor fence values changed to 2x manufacture cost (without cloth)
        case ARMOR_CLOTHES:
            fenceamount = 20;
            break;

        case ARMOR_OVERALLS:
            fenceamount = 10;
            break;

        case ARMOR_WIFEBEATER:
            fenceamount = 4;
            break;

        case ARMOR_TRENCHCOAT:
            fenceamount = 40;
            break;

        case ARMOR_WORKCLOTHES:
            fenceamount = 20;
            break;

        case ARMOR_SECURITYUNIFORM:
            fenceamount = 80;
            break;

        case ARMOR_POLICEUNIFORM:
            fenceamount = 80;
            break;

        case ARMOR_DEATHSQUADUNIFORM:
            fenceamount = 80;
            break;

        case ARMOR_CHEAPSUIT:
            fenceamount = 100;
            break;

        case ARMOR_EXPENSIVESUIT:
            fenceamount = 600;
            break;

        case ARMOR_BLACKSUIT:
            fenceamount = 120;
            break;

        case ARMOR_CHEAPDRESS:
            fenceamount = 40;
            break;

        case ARMOR_EXPENSIVEDRESS:
            fenceamount = 600;
            break;

        case ARMOR_BLACKDRESS:
            fenceamount = 120;
            break;

        case ARMOR_LABCOAT:
            fenceamount = 40;
            break;

        case ARMOR_BLACKROBE:
            fenceamount = 40;
            break;

        case ARMOR_CLOWNSUIT:
            fenceamount = 40;
            break;

        case ARMOR_BONDAGEGEAR:
            fenceamount = 60;
            break;

        case ARMOR_MASK:
            fenceamount = 10;
            break;

        case ARMOR_MILITARY:
            fenceamount = 80;
            break;

        case ARMOR_PRISONGUARD:
            fenceamount = 80;
            break;

        case ARMOR_PRISONER:
            fenceamount = 40;
            break;

        case ARMOR_TOGA:
            fenceamount = 10;
            break;

        case ARMOR_MITHRIL:
            fenceamount = 50;
            break;

        case ARMOR_CIVILLIANARMOR:
            fenceamount = 75;
            break;

        case ARMOR_POLICEARMOR:
            fenceamount = 150;
            break;

        case ARMOR_SWATARMOR:
            fenceamount = 200;
            break;

        case ARMOR_ARMYARMOR:
            fenceamount = 250;
            break;

        case ARMOR_HEAVYARMOR:
            fenceamount = 400;
            break;
        }

        // Sell second-rate clothing for second-rate prices
        fenceamount >>= it.armor.quality - '1';
        break;

    case ITEM_CLIP:
        switch(it.cliptype) {
        case CLIP_38:
            fenceamount = 2;
            break;

        case CLIP_44:
            fenceamount = 3;
            break;

        case CLIP_45:
            fenceamount = 4;
            break;

        case CLIP_9:
            fenceamount = 4;
            break;

        case CLIP_ASSAULT:
            fenceamount = 8;
            break;

        case CLIP_SMG:
            fenceamount = 5;
            break;

        case CLIP_BUCKSHOT:
            fenceamount = 3;
            break;

        case CLIP_MOLOTOV:
            fenceamount = 2;
            break;
        }

        break;

    case ITEM_LOOT:
        switch(it.loottype) {
        case LOOT_KIDART:
            fenceamount = 1;
            break;

        case LOOT_FAMILYPHOTO:
            fenceamount = 1;
            break;

        case LOOT_DIRTYSOCK:
            fenceamount = 1;
            break;

        case LOOT_LABEQUIPMENT:
            fenceamount = 50;
            break;

        case LOOT_COMPUTER:
            fenceamount = 200;
            break;

        case LOOT_WATCH:
            fenceamount = 20;
            break;

        case LOOT_PDA:
            fenceamount = 50;
            break;

        case LOOT_CELLPHONE:
            fenceamount = 20;
            break;

        case LOOT_MICROPHONE:
            fenceamount = 20;
            break;

        case LOOT_TRINKET:
            fenceamount = 5;
            break;

        case LOOT_SILVERWARE:
            fenceamount = 20;
            break;

        case LOOT_CHEAPJEWELERY:
            fenceamount = 50;
            break;

        case LOOT_EXPENSIVEJEWELERY:
            fenceamount = 500;
            break;

        case LOOT_FINECLOTH:
            fenceamount = 20;
            break;

        case LOOT_CHEMICAL:
            fenceamount = 20;
            break;

        case LOOT_CEOPHOTOS:
            fenceamount = 1000;
            break;

        case LOOT_CEOLOVELETTERS:
            fenceamount = 250;
            break;

        case LOOT_CEOTAXPAPERS:
            fenceamount = 1000;
            break;

        case LOOT_SECRETDOCUMENTS:
            fenceamount = 1500;
            break;

        case LOOT_INTHQDISK:
            fenceamount = 1500;
            break;

        case LOOT_CORPFILES:
            fenceamount = 1000;
            break;

        case LOOT_POLICERECORDS:
            fenceamount = 750;
            break;

        case LOOT_JUDGEFILES:
            fenceamount = 1000;
            break;

        case LOOT_RESEARCHFILES:
            fenceamount = 750;
            break;

        case LOOT_PRISONFILES:
            fenceamount = 750;
            break;

        case LOOT_CABLENEWSFILES:
            fenceamount = 250;
            break;

        case LOOT_AMRADIOFILES:
            fenceamount = 250;
            break;
        }

        break;
    }

    return fenceamount;
}



/* choose buyer */
void choose_buyer(short &buyer) {
    if(activesquad == NULL)
        return;

    party_status = -1;

    int partysize = 0;

    for(int p = 0; p < 6; p++) {
        if(activesquad->squad[p] != NULL)
            partysize++;
    }

    if(partysize <= 1)
        return;

    do {
        printparty();

        move(8, 20);
        set_color(COLOR_WHITE, COLOR_BLACK, 1);
        addstr("Choose a Liberal squad member to SPEND.");

        refresh();

        int c = getch();
        translategetch(c);

        if(c == 10)
            return;

        if(c >= '1' && c <= partysize + '1' - 1) {
            buyer = c - '1';
            return;
        }
    } while(1);
}
