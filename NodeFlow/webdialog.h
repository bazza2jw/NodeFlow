#ifndef WEBDIALOG_H
#define WEBDIALOG_H

#include <MrlLib/mrllib.h>
#include <MrlLib/variant.h>
#include <MrlLib/variantpropertytree.h>
#include <Wt/WWidget.h>
#include <Wt/WDialog.h>
#include <Wt/WLabel.h>
#include <Wt/WPushButton.h>
#include <Wt/WApplication.h>
#include <Wt/WTextEdit.h>
#include <Wt/WGridLayout.h>
#include <Wt/WLineEdit.h>
#include <Wt/WTable.h>

namespace NODEFLOW {
    /*!
           \brief The WebModalDialogBase class
    */

//
// Translation macro for Wt widgets - Wt does have Wt::WString::tr() there is also the wxWidgets translation engine
// Better to use one rather than two ?
//
//#define _TR(s) wxGetTranslation((s)).ToStdString()
#define  _TR(s)  (s)
//Wt::WString::tr(s)


      #define SCROLL_TABLE_CSS ".tableFixHead {overflow-y: auto; height: 106px; }" \
      ".tableFixHead thead th { position: sticky; top: 0; }"\
      "table { border-collapse: collapse; width: 100%; }"

    class WebDialogBase : public Wt::WDialog {
protected:
            bool _okClicked = false;
        public:
            WebDialogBase() {}
            virtual void onOk() { }
            virtual void setup() {}
            bool okClicked() const { return _okClicked;}

            template <typename T>
            /*!
             * \brief show
             * \param dialog
             */
            static void doDialog(Wt::WWidget *owner,T * dialog)
            {

                dialog->setClosable(true);
                dialog->setResizable(true);
                dialog->setTitleBarEnabled(true);
                dialog->setMovable(true);
                dialog->setVerticalAlignment (Wt::AlignmentFlag::Middle);
                Wt::WPushButton *ok = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("OK")));
                ok->setDefault(true);
                //if (wApp->environment().ajax())
                //    ok->disable();
                Wt::WPushButton *cancel = dialog->footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("Cancel")));
                dialog->rejectWhenEscapePressed();
                dialog->setup();
                dialog->_okClicked = false;
                dialog->contents()->addStyleClass("form-group");
                /*
                    Accept the dialog
                */
                ok->clicked().connect([ = ] {
                    dialog->accept();
                });

                /*
                    Reject the dialog
                */
                cancel->clicked().connect(dialog, &Wt::WDialog::reject);


                /*
                    Process the dialog result.
                */
                dialog->finished().connect([ = ] {
                    if (dialog->result() == Wt::DialogCode::Accepted) {
                        dialog->_okClicked = true;
                        dialog->onOk();
                    }
                    owner->removeChild(dialog); // deletes the dialog object
                });

                dialog->show();

            }

            /*!
             * \brief show
             * \param dialog
             */
            bool doExec()
            {
                setClosable(true);
                setResizable(true);
                setTitleBarEnabled(true);
                setMovable(true);
                setVerticalAlignment (Wt::AlignmentFlag::Middle);
                Wt::WPushButton *ok = footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("OK")));
                ok->setDefault(true);
                Wt::WPushButton *cancel = footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("Cancel")));
                cancel->clicked().connect(this, &Wt::WDialog::reject);
                ok->clicked().connect(this,&Wt::WDialog::accept);
                rejectWhenEscapePressed();
                setup(); // call the set up function
                bool res = (exec() == Wt::DialogCode::Accepted); // use of exec
                if(res) onOk(); // read from fields to class
                return res;
             }


            template <typename T>
            /*!
                \brief showDialog
                \param owner
                \param p
            */
            static void showDialog(Wt::WWidget *owner, std::unique_ptr<T> &p) {
                auto dialog = owner->addChild(std::move(p));
                doDialog(owner,dialog);
            }


            template <typename T, typename A>
            /*!
                \brief showDialog
                \param owner
                \param p
            */
            static void showDialog(Wt::WWidget *owner, A * arg) {
                auto dialog = owner->addChild(std::make_unique<T>(arg));
                doDialog(owner,dialog);
            }



            template <typename T>
            /*!
                \brief showDialog
                \param owner
                \param p
            */
            static void showDialog(Wt::WWidget *owner) {
                auto dialog = owner->addChild(std::make_unique<T>());
                doDialog(owner,dialog);
            }

            template <typename T>
            /*!
                \brief showDialog
                \param owner
                \param p
            */
            static void showDialog(Wt::WWidget *owner, const MRL::PropertyPath &p) {
                auto dialog = owner->addChild(std::make_unique<T>(p));
                doDialog(owner,dialog);
            }


            template <typename T>
            /*!
             * \brief showDialog
             * Use this for dialogs called from a setup dialog where the same config is shared between them all
             * \param owner
             * \param c
             * \param p
             */
            static void showDialog(Wt::WWidget *owner, MRL::VariantPropertyTree &c, MRL::PropertyPath &p) {
                auto dialog = owner->addChild(std::make_unique<T>(c,p));
                doDialog(owner,dialog);
            }

            template <typename T>
            /*!
             * \brief addToTable
             * \param t
             * \param row
             * \param col
             * \param p
             * \return
             */
            static T * addToTable(Wt::WTable * t , int row, int col, T * &p)
            {
                if(t)
                {
                    // add a control
                    p = t->elementAt(row, col)->addWidget(std::make_unique<T>());
                    return p;
                }
                return nullptr;
            }
            template <typename T>
            /*!
             * \brief addLabledToTable
             * \param t
             * \param s
             * \param row
             * \param col
             * \param p
             * \return
             */
            static T * addLabeledToTable(Wt::WTable * t , const std::string &s, int row, int col, T * &p)
            {
                // add a label and a control to a table
                if(t)
                {
                    t->elementAt(row, col)->addWidget(std::make_unique<Wt::WLabel>(_TR(s)));
                    p = t->elementAt(row, col + 1)->addWidget(std::make_unique<T>());
                    return p;
                }
                return nullptr;
            }

    };

    /*!
     * \brief The WebGetInput class
     */
    class WebGetInput : public Wt::WDialog
    {
        Wt::WString _prompt;
        Wt::WLineEdit * _input = nullptr;
    public:
        WebGetInput(const Wt::WString &s) : _prompt(s)
        {
            setup();
        }

        void setup()
        {
            setClosable(true);
            setResizable(true);
            setTitleBarEnabled(true);
            setMovable(true);
            setVerticalAlignment (Wt::AlignmentFlag::Middle);
            Wt::WPushButton *ok = footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("OK")));
            ok->setDefault(true);
            //if (wApp->environment().ajax())
            //    ok->disable();
            Wt::WPushButton *cancel = footer()->addWidget(std::make_unique<Wt::WPushButton>(_TR("Cancel")));
            rejectWhenEscapePressed();
            auto layout = contents()->setLayout(std::make_unique<Wt::WGridLayout>()); // a bag grid layout
            // add the items to the grid
            layout->addWidget(std::make_unique<Wt::WLabel>(_prompt), 0, 0);
            _input = layout->addWidget(std::make_unique<Wt::WLineEdit>(), 0, 1);
            /*
                Accept the dialog
            */
            ok->clicked().connect(this, &Wt::WDialog::accept);

            /*
                Reject the dialog
            */
            cancel->clicked().connect(this, &Wt::WDialog::reject);

        }

        void ok()
        {

        }
        /*!
         * \brief input
         * \return
         */
        const Wt::WString &input() { return _input->text();}
        /*!
         * \brief getInput
         * \param owner
         * \param prompt
         * \param resString
         * \return
         */
        static bool getInput(Wt::WWidget *owner , const Wt::WString &prompt, Wt::WString &resString)
        {
            WebGetInput dlg(prompt);
            bool res = dlg.exec() == Wt::DialogCode::Accepted;
            resString = dlg.input();
            return res;
        }

    };


}

#endif // WEBDIALOG_H
