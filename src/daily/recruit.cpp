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

//#include "includes.h"
#include "externs.h"


/* recruit struct constructor */
recruitst::recruitst() : timeleft(0), level(0), eagerness1(0), task(0) {
    //Has heard of the LCS
    if((int)LCSrandom(100) < attitude[VIEW_LIBERALCRIMESQUAD]) {
        //Likes the LCS
        if((int)LCSrandom(100) < attitude[VIEW_LIBERALCRIMESQUADPOS])
            eagerness1 = 3;
        //Doesn't like the LCS
        else
            eagerness1 = 0;
    } else
        eagerness1 = 2;

}

recruitst::~recruitst() {
    delete recruit;
}

char recruitst::eagerness() {
    char eagerness_temp = eagerness1;

    //Moderates are decidedly less interested
    if(recruit->align == 0)
        eagerness_temp -= 2;

    //Conservatives are extremely uninterested
    if(recruit->align == -1)
        eagerness_temp -= 4;

    return eagerness_temp;
}

static void getissueeventstring(char *str) {
    switch(LCSrandom(VIEWNUM - 3)) {
    case VIEW_DRUGS:
        strcat(str, "a collection of studies on the health effects of marijuana");
        break;

    case VIEW_MILITARY:
        strcat(str, "a book on the history of military atrocities");
        break;

    case VIEW_IMMIGRATION:
        strcat(str, "a reality TV episode on the lives of immigrants");
        break;

    case VIEW_CIVILRIGHTS:
        strcat(str, "a documentary on the civil rights struggle");
        break;

    case VIEW_WOMEN:
        strcat(str, "a documentary on the women's rights struggle");
        break;

    case VIEW_GAY:
        strcat(str, "a documentary on the gay rights struggle");
        break;

    case VIEW_DEATHPENALTY:
        strcat(str, "a research paper on abuses of the death penalty");
        break;

    case VIEW_TAXES:
        strcat(str, "an economic paper on the flaws of trickle-down");
        break;

    case VIEW_NUCLEARPOWER:
        strcat(str, "a video tour of the Chernobyl dead zone");
        break;

    case VIEW_ANIMALRESEARCH:
        strcat(str, "a documentary on animal research");
        break;

    case VIEW_POLICEBEHAVIOR:
        strcat(str, "a hand-recorded video of police brutality");
        break;

    case VIEW_TORTURE:
        strcat(str, "a government inquiry into military interrogations");
        break;

    case VIEW_PRISONS:
        strcat(str, "a documentary on the prisoners' suffering");
        break;

    case VIEW_INTELLIGENCE:
        strcat(str, "a documentary on privacy rights");
        break;

    case VIEW_FREESPEECH:
        strcat(str, "a collection of banned books");
        break;

    case VIEW_GENETICS:
        strcat(str, "a video about genetic engineering accidents");
        break;

    case VIEW_JUSTICES:
        strcat(str, "a Liberal policy paper inquiring into judicial decisions");
        break;

    case VIEW_GUNCONTROL:
        strcat(str, "a book profiling school shootings");
        break;

    case VIEW_SWEATSHOPS:
        strcat(str, "a hand-recorded video of unregulated sweatshops");
        break;

    case VIEW_POLLUTION:
        strcat(str, "a leaked government paper on environmental conditions");
        break;

    case VIEW_CORPORATECULTURE:
        strcat(str, "a documentary on life under corporate culture");
        break;

    case VIEW_CEOSALARY:
        strcat(str, "a Liberal think-tank survey of top CEO salaries");
        break;

    case VIEW_AMRADIO:
        strcat(str, "a collection of Conservative radio host rants");
        break;

    case VIEW_CABLENEWS:
        strcat(str, "a collection of leaked Conservative cable news memos");
        break;
        //case VIEW_POLITICALVIOLENCE:strcat(str,"a documentary about progress made by direct action");break;
    }
}

/* recruiting */
char recruitment_activity(Creature &cr, char &clearformess) {
    clearformess = 1;
    int ocursite = cursite;
    cursite = cr.location;

    int type = cr.activity.arg;
    int difficulty = recruitFindDifficulty(type);
    char *name = recruitName(type);

    if(type >= 0) {
        cr.train(SKILL_STREETSENSE, 5);

        erase();
        set_color(COLOR_WHITE, COLOR_BLACK, 1);
        mvaddstr(0, 0, "Adventures in Liberal Recruitment");
        printcreatureinfo(&cr);
        makedelimiter(8, 0);

        set_color(COLOR_WHITE, COLOR_BLACK, 0);
        mvaddstr_f(10, 0, "%s asks around for a %s...", cr.name, name);

        refresh();
        getch();

        int recruitCount = 0;

        if(difficulty < 10) {
            // Generate recruitment candidates
            for(recruitCount = 0; recruitCount < 5; recruitCount++) {
                if(recruitCount == 0 || cr.skill_roll(SKILL_STREETSENSE) > (difficulty + recruitCount * 2)) {
                    makecreature(encounter[recruitCount], type);
                    encounter[recruitCount].namecreature();
                } else
                    break;
            }
        }

        if(recruitCount == 0) {
            mvaddstr_f(11, 0, "%s was unable to track down a %s.", cr.name, name);
            getch();
            cursite = ocursite;
            return 0;
        }

        if(recruitCount == 1) {
            erase();
            set_color(COLOR_WHITE, COLOR_BLACK, 1);
            mvaddstr(0, 0, "Adventures in Liberal Recruitment");
            printcreatureinfo(&cr);
            makedelimiter(8, 0);

            set_color(COLOR_WHITE, COLOR_BLACK, 1);
            mvaddstr_f(11, 0, "%s managed to set up a meeting with ", cr.name);
            set_alignment_color(encounter[0].align);
            addstr(encounter[0].name);
            add_age(encounter[0]);
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            addstr(".");
            getch();

            erase();
            set_color(COLOR_WHITE, COLOR_BLACK, 1);
            mvaddstr(0, 0, "Adventures in Liberal Recruitment");
            printcreatureinfo(&encounter[0]);
            makedelimiter(8, 0);
            talk(cr, 0);
        } else {
            while(recruitCount > 0) {
                if(recruitCount == 1) {
                    erase();
                    set_color(COLOR_WHITE, COLOR_BLACK, 1);
                    mvaddstr(0, 0, "Adventures in Liberal Recruitment");
                    printcreatureinfo(&cr);
                    makedelimiter(8, 0);

                    set_color(COLOR_WHITE, COLOR_BLACK, 1);
                    mvaddstr_f(11, 0, "%s approaches the last %s.", cr.name, name);
                    //set_alignment_color(encounter[0].align);
                    //addstr(encounter[0].name);
                    //add_age(encounter[0]);
                    //set_color(COLOR_WHITE,COLOR_BLACK,0);
                    //addstr(".");
                    getch();

                    erase();
                    set_color(COLOR_WHITE, COLOR_BLACK, 1);
                    mvaddstr(0, 0, "Adventures in Liberal Recruitment");
                    printcreatureinfo(&encounter[0]);
                    makedelimiter(8, 0);
                    talk(cr, 0);
                    --recruitCount;
                } else {
                    erase();
                    set_color(COLOR_WHITE, COLOR_BLACK, 1);
                    mvaddstr(0, 0, "Adventures in Liberal Recruitment");
                    printcreatureinfo(&cr);
                    makedelimiter(8, 0);

                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    mvaddstr_f(10, 0, "%s was able to get information on multiple people.", cr.name);

                    for(int i = 0; i < recruitCount; i++) {
                        set_color(COLOR_WHITE, COLOR_BLACK, 0);
                        mvaddstr_f(12 + i, 0, "%c - ", 'A' + i);
                        set_alignment_color(encounter[i].align);
                        addstr(encounter[i].name);
                        add_age(encounter[i]);
                    }

                    set_color(COLOR_WHITE, COLOR_BLACK, 0);
                    mvaddstr(12 + recruitCount + 1, 0, "Press enter or escape to call it a day.");
                    int c = getch();
                    translategetch(c);

                    if(c == ENTER || c == ESC)
                        break;

                    c -= 'a';

                    if(c >= 0 && c < ENCMAX - 1 && encounter[c].exists) {
                        int id = encounter[c].id;
                        erase();
                        set_color(COLOR_WHITE, COLOR_BLACK, 1);
                        mvaddstr(0, 0, "Adventures in Liberal Recruitment");
                        printcreatureinfo(&encounter[c]);
                        makedelimiter(8, 0);

                        talk(cr, c);

                        if(encounter[c].id == id)
                            delenc(c, 0);

                        --recruitCount;
                    }
                }
            }
        }
    }

    cursite = ocursite;
    return 1;
}

/* daily - recruit - recruit meeting */
char completerecruitmeeting(recruitst &r, int p, char &clearformess) {
    char num[5]; // for temporary use for itoa()
    clearformess = 1;

    erase();
    set_color(COLOR_WHITE, COLOR_BLACK, 1);
    move(0, 0);

    if(pool[p]->meetings++ > 5 && LCSrandom(pool[p]->meetings - 5)) {
        addstr(pool[p]->name, gamelog);
        addstr(" accidentally missed the meeting with ", gamelog);
        addstr(r.recruit->name, gamelog);
        move(1, 0);
        addstr("due to multiple booking of recruitment sessions.", gamelog);
        gamelog.newline();

        move(3, 0);
        addstr("Get it together, ", gamelog);
        addstr(pool[p]->name, gamelog);
        addstr("!", gamelog);
        gamelog.nextMessage();
        getch();

        return 1;
    }

    addstr("Meeting with ", gamelog);
    addstr(r.recruit->name, gamelog);
    addstr(", ", gamelog);
    addstr(r.recruit->get_type_name(), gamelog);
    addstr(", ", gamelog);
    addstr(location[r.recruit->location]->name, gamelog);
    gamelog.newline();

    set_color(COLOR_WHITE, COLOR_BLACK, 0);
    printfunds(0, 1, "Money: ");

    printcreatureinfo(r.recruit);
    makedelimiter(8, 0);

    move(10, 0);
    addstr(r.recruit->name);

    switch(r.eagerness()) {
    case 1:
        set_color(COLOR_YELLOW, COLOR_BLACK, 0);
        addstr(" will take a lot of persuading.");
        break;

    case 2:
        set_color(COLOR_CYAN, COLOR_BLACK, 0);
        addstr(" is interested in learning more.");
        break;

    case 3:
        set_color(COLOR_BLUE, COLOR_BLACK, 1);
        addstr(" feels something needs to be done.");
        break;

    default:
        if(r.eagerness() >= 4) {
            set_color(COLOR_GREEN, COLOR_BLACK, 1);
            addstr(" is ready to fight for the Liberal Cause.");
        }
        else {
            set_color(COLOR_MAGENTA, COLOR_BLACK, 1);
            addstr(" kind of regrets agreeing to this.");
        }

        break;
    }
    
    set_color(COLOR_WHITE, COLOR_BLACK, 0);
    move(11, 0);
    addstr("How should ");
    print_name_colored_according_to_juice(pool[p]);
    addstr(" [");
    itoa(maxsubordinates(*pool[p]) - subordinatesleft(*pool[p]), num, 10);
    addstr(num);
    addstr("/");
    itoa(maxsubordinates(*pool[p]), num, 10);
    addstr(num);
    addstr(" recruits] approach the situation?");

    move(13, 0);

    if(ledger.get_funds() < 50)
        set_color(COLOR_BLACK, COLOR_BLACK, 1);

    addstr("A - Spend $50 on props and a book for them to keep afterward.");
    set_color(COLOR_WHITE, COLOR_BLACK, 0);
    move(14, 0);
    addstr("B - Just casually chat with them and discuss politics.");

    move(15, 0);

    if(subordinatesleft(*pool[p]) && r.eagerness() >= 4) {
        addstr("C - Offer to let ");
        addstr(r.recruit->name);
        addstr(" to join the LCS as a full member.");
    } else if(!subordinatesleft(*pool[p])) {
        set_color(COLOR_BLACK, COLOR_BLACK, 1);
        addstr("C - ");
        addstr(pool[p]->name);
        addstr(" needs more Juice to recruit.");
        set_color(COLOR_WHITE, COLOR_BLACK, 0);
    } else {
        set_color(COLOR_BLACK, COLOR_BLACK, 1);
        addstr("C - ");
        addstr(r.recruit->name);
        addstr(" isn't ready to join the LCS.");
        set_color(COLOR_WHITE, COLOR_BLACK, 0);
    }

    move(16, 0);
    addstr("D - Break off the meetings.");

    int y = 18;


    do {
        refresh();
        int c = getch();
        translategetch(c);

        if(c == 'c' && subordinatesleft(*pool[p]) && r.eagerness() >= 4) {
            move(y, 0);
            addstr(pool[p]->name, gamelog);
            addstr(" offers to let ", gamelog);
            addstr(r.recruit->name, gamelog);
            addstr(" join the Liberal Crime Squad.", gamelog);
            gamelog.newline();

            refresh();
            getch();

            set_color(COLOR_GREEN, COLOR_BLACK, 1);
            move(y += 2, 0);

            addstr(r.recruit->name, gamelog);
            addstr(" accepts, and is eager to get started.", gamelog);
            gamelog.nextMessage();

            liberalize(*r.recruit, false);

            refresh();
            getch();
            
            erase();
            
            move(2, 0);
            set_color(COLOR_WHITE, COLOR_BLACK, 0);
            addstr("What name will you use for this ");
            addstr(r.recruit->get_type_name());
            addstr(" in its presence?");
            move(3, 0);
            addstr("If you do not enter anything, their real name will be used.");

            move(4, 0);
            enter_name(r.recruit->name, CREATURE_NAMELEN, r.recruit->propername);
            
            sleeperize_prompt(*r.recruit, *pool[p], 6);

            r.recruit->hireid = pool[p]->id;

            pool[p]->train(SKILL_PERSUASION, 25);

            pool.push_back(r.recruit);
            r.recruit = NULL;
            stat_recruits++;

            return 1;
        }

        if(c == 'b' || (c == 'a' && ledger.get_funds() >= 50)) {
            if(c == 'a')
                ledger.subtract_funds(50, EXPENSE_RECRUITMENT);

            pool[p]->train(SKILL_PERSUASION,
                           max(12 - pool[p]->get_skill(SKILL_PERSUASION), 5));
            pool[p]->train(SKILL_SCIENCE,
                           max(r.recruit->get_skill(SKILL_SCIENCE) - pool[p]->get_skill(SKILL_SCIENCE), 0));
            pool[p]->train(SKILL_RELIGION,
                           max(r.recruit->get_skill(SKILL_RELIGION) - pool[p]->get_skill(SKILL_RELIGION), 0));
            pool[p]->train(SKILL_LAW,
                           max(r.recruit->get_skill(SKILL_LAW) - pool[p]->get_skill(SKILL_LAW), 0));
            pool[p]->train(SKILL_BUSINESS,
                           max(r.recruit->get_skill(SKILL_BUSINESS) - pool[p]->get_skill(SKILL_BUSINESS), 0));

            int lib_persuasiveness = pool[p]->get_skill(SKILL_BUSINESS) +
                                     pool[p]->get_skill(SKILL_SCIENCE) +
                                     pool[p]->get_skill(SKILL_RELIGION) +
                                     pool[p]->get_skill(SKILL_LAW) +
                                     pool[p]->get_attribute(ATTRIBUTE_INTELLIGENCE, true);

            int recruit_reluctance = 5 +
                                     r.recruit->get_skill(SKILL_BUSINESS) +
                                     r.recruit->get_skill(SKILL_SCIENCE) +
                                     r.recruit->get_skill(SKILL_RELIGION) +
                                     r.recruit->get_skill(SKILL_LAW) +
                                     r.recruit->get_attribute(ATTRIBUTE_WISDOM, true) +
                                     r.recruit->get_attribute(ATTRIBUTE_INTELLIGENCE, true);

            if(lib_persuasiveness > recruit_reluctance)
                recruit_reluctance = 0;
            else
                recruit_reluctance -= lib_persuasiveness;

            int difficulty = recruit_reluctance;

            char str[75];
            strcpy(str, "");

            if(c == 'a') {
                difficulty -= 5;

                move(y++, 0);
                addstr(pool[p]->name, gamelog);
                addstr(" shares ", gamelog);
                getissueeventstring(str);
                addstr(str, gamelog);
                addstr(".", gamelog);
                gamelog.newline();

                refresh();
                getch();
            } else {
                move(y++, 0);
                addstr(pool[p]->name, gamelog);
                addstr(" explains ", gamelog);
                addstr(pool[p]->hisher(), gamelog);
                addstr(" views on ", gamelog);
                getviewsmall(str, LCSrandom(VIEWNUM - 3));
                addstr(str, gamelog);
                addstr(".", gamelog);
                gamelog.newline();
                refresh();
                getch();
            }

            //bool success=0;

            if(pool[p]->skill_check(SKILL_PERSUASION, difficulty)) {
                //success=1;
                set_color(COLOR_CYAN, COLOR_BLACK, 1);

                if(r.level < 127)
                    r.level++;

                if(r.eagerness1 < 127)
                    r.eagerness1++;

                move(y++, 0);
                addstr(r.recruit->name, gamelog);
                addstr(" found ", gamelog);
                addstr(pool[p]->name, gamelog);
                addstr("'s views to be insightful.", gamelog);
                gamelog.newline();
                move(y++, 0);
                addstr("They'll definitely meet again tomorrow.", gamelog);
                gamelog.nextMessage();
            } else if(pool[p]->skill_check(SKILL_PERSUASION, difficulty)) { // Second chance to not fail horribly
                if(r.level < 127)
                    r.level++;

                if(r.eagerness1 > -128)
                    r.eagerness1--;

                move(y++, 0);
                addstr(r.recruit->name, gamelog);
                addstr(" is skeptical about some of ", gamelog);
                addstr(pool[p]->name, gamelog);
                addstr("'s arguments.", gamelog);
                gamelog.newline();
                move(y++, 0);
                addstr("They'll meet again tomorrow.", gamelog);
                gamelog.nextMessage();
            } else {
                set_color(COLOR_MAGENTA, COLOR_BLACK, 1);
                move(y++, 0);

                if(r.recruit->talkreceptive() && r.recruit->align == ALIGN_LIBERAL) {
                    addstr(r.recruit->name, gamelog);
                    addstr(" isn't convinced ", gamelog);
                    addstr(pool[p]->name, gamelog);
                    addstr(" really understands the problem.", gamelog);
                    gamelog.newline();
                    move(y++, 0);
                    addstr("Maybe ", gamelog);
                    addstr(pool[p]->name, gamelog);
                    addstr(" needs more experience.", gamelog);
                    gamelog.nextMessage();
                } else {
                    addstr(pool[p]->name, gamelog);
                    addstr(" comes off as slightly insane.", gamelog);
                    gamelog.newline();
                    move(y++, 0);
                    addstr("This whole thing was a mistake. There won't be another meeting.", gamelog);
                    gamelog.nextMessage();
                }

                refresh();
                getch();
                return 1;
            }

            refresh();
            getch();
            return 0;
        }

        if(c == 'd')
            return 1;
    } while(1);
}
