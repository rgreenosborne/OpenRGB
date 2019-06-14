#include "OpenAuraSDKDialog.h"
#include "OpenAuraSDK.h"
#include "I2CDetectDialog.h"

IMPLEMENT_DYNAMIC(OpenAuraSDKDialog, CDialogEx)

OpenAuraSDKDialog::OpenAuraSDKDialog(std::vector<i2c_smbus_interface *>& bus, std::vector<RGBController *>& control, CWnd* pParent)
    : CDialogEx(IDD_DIALOG_OPENAURASDK, pParent), busses(bus), controllers (control)
{

}

OpenAuraSDKDialog::~OpenAuraSDKDialog()
{

}

void OpenAuraSDKDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(OpenAuraSDKDialog, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_I2CDETECT, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkI2cdetect)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColors)
    ON_CBN_CLOSEUP(IDC_COMBO_OPENAURASDK_DEVICE, &OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_SET_COLORS_ALL, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll)
    ON_BN_CLICKED(IDC_BUTTON_OPENAURASDK_DUMP, &OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkDump)
    ON_CBN_CLOSEUP(IDC_COMBO_OPENAURASDK_MODE, &OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkMode)
END_MESSAGE_MAP()

BOOL OpenAuraSDKDialog::OnInitDialog()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);

    for (int i = 0; i < controllers.size(); i++)
    {
        controller_box->AddString(controllers[i]->name.c_str());
    }

    controller_box->SetCurSel(0);

    for (int i = 0; i < controllers[0]->modes.size(); i++)
    {
        mode_box->AddString(controllers[0]->modes[i].name.c_str());
    }

    mode_box->SetCurSel(0);

    return TRUE;
}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkI2cdetect()
{
    I2CDetectDialog dlg(busses);
    dlg.DoModal();
}

void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColors()
{

}


void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkSetColorsAll()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);

    controllers[controller_box->GetCurSel()]->SetAllLEDs(
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_R),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_G),
        GetDlgItemInt(IDC_EDIT_OPENAURASDK_LED_0_B)
    );
}


void OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkDevice()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);

    mode_box->ResetContent();

    for (int i = 0; i < controllers[controller_box->GetCurSel()]->modes.size(); i++)
    {
        mode_box->AddString(controllers[controller_box->GetCurSel()]->modes[i].name.c_str());
    }

    mode_box->SetCurSel(controllers[controller_box->GetCurSel()]->GetMode());
}

void OpenAuraSDKDialog::OnBnClickedButtonOpenaurasdkDump()
{

}


void OpenAuraSDKDialog::OnCbnCloseupComboOpenaurasdkMode()
{
    CComboBox* controller_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_DEVICE);
    CComboBox* mode_box = (CComboBox*)GetDlgItem(IDC_COMBO_OPENAURASDK_MODE);

    controllers[controller_box->GetCurSel()]->SetMode(mode_box->GetCurSel());
}