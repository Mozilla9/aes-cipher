/******************************************************************************/
/*  Class SFILE                                                               */
/*  PO: S. Maslyakov, rusoil.9@gmail.com                                      */
/*                                                                            */
/*  Revision:     1.0                                                         */
/*  Date:         2011/02/05 17:02:33                                         */
/******************************************************************************/


#ifndef __SFILE_H
#define __SFILE_H



//==============================================================================
//  Class SFILE
//==============================================================================
class SFILE
{
public:
    SFILE(const std::string _name) : m_filename(_name) {}

    virtual ~SFILE() {
        if (m_file.is_open()) {
            m_file.close();
        };
    }

    uint32_t Open() {
        m_file.open(m_filename.c_str(), std::ios::in|std::ios::out|std::ios::trunc);

        return m_file.is_open();
    }

    std::fstream& GetFile() {
        return m_file;
    }

private:
    std::fstream        m_file;
    const std::string   m_filename;
};
//==============================================================================
//==============================================================================


#endif